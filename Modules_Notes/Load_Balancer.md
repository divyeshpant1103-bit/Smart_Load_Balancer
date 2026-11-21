# Load Balancer Module

## 1. What This Module Does

The Load Balancer decides which server receives each incoming request. It is the routing layer between the clients and the cluster.

For every request it looks at the active servers, picks the one with the most free capacity, and assigns the request there. If no server has room, the request waits in the `RequestQueue`. On every simulation tick it also moves finished work out of the servers, routes waiting requests into the freed capacity, and asks the `autoScaler` whether the cluster should grow or shrink.

The module has two parts:

| Part | Files | Role |
|---|---|---|
| `Loadbalancer` | `include/load_balancer.hpp`, `src/loadbalancer.cpp` | Routing, queue fallback, tick processing, counters |
| `ServerMaxHeap` | `include/servermaxheap.hpp` | Hand-built max-heap that ranks servers by free capacity |

## 2. Key Design Decisions

- **Hand-built max-heap, not `std::priority_queue`.** The project builds its own merge sort in the report generator, so the routing structure is also written by hand. The ranking rule lives inside the heap (`isHigherPriority`), so there is no separate comparator class.
- **Ranking key is free capacity.** Free capacity is `max capacity - current load`. The server with the most free capacity is at the root.
- **Ties go to the lowest server ID.** Without a tie rule, two equal servers would be picked in an arbitrary order. With it, the result is deterministic: three fresh servers always give the first request to Server 1.
- **A fresh heap is built on every routing call.** Server loads change after every assignment and every tick, and a heap cannot update the priority of an element that is already inside it. A long-lived heap would go stale and keep picking the wrong server. Rebuilding costs O(n log n) per call, which is small here because the cluster is capped at 6 servers.
- **Only servers with room enter the heap.** A server is inserted only if `current load < max capacity`. An empty heap therefore means "no server can take this request".
- **`extractMax()` returns `nullptr` on an empty heap.** One null check covers both "no active servers" and "every server is full". There is no separate emptiness check that could go out of sync.
- **Queue fallback keeps order.** A new request that cannot be placed goes to the back of the queue with `enqueue()`. During draining, a request that still cannot be placed goes back to the front with `enqueue_front()`, so it keeps its position and recovered requests stay first.
- **Encapsulation.** The load balancer never touches the cluster's list pointers. It works on the `std::vector<Server*>` snapshot returned by `getActiveServers()`.
- **No internal locking.** The class does not lock anything. The caller (`main.cpp`) holds one shared mutex around every call into it.

## 3. How a Request Is Routed

`receiveRequest(req)`

1. Increase `total_requests_received`.
2. Call `tryAssign(req)`.
3. If `tryAssign` returns `false`, call `enqueue(req)` on the queue.

`tryAssign(req)`

1. Take the snapshot from `getActiveServers()`.
2. Insert every server with free capacity into a new `ServerMaxHeap`.
3. `extractMax()` gives the best server, or `nullptr` if there is none (return `false`).
4. Return the result of `best->assignRequest(req)`. The server checks its own status and capacity again, so a rejection also returns `false`.

## 4. What Happens on Each Tick

`processTick()`

1. Take the active servers and tick each one.
2. Pop every completed request from each server and increase `total_requests_processed`.
3. `drainQueue()`: take requests from the front of the queue and route them, stopping at the first one that cannot be placed.
4. Ask `autoScaler::checkScaling(servers, queueSize)` for a decision.
5. On `SCALE_UP`, add a server and drain the queue again so waiting requests use it at once. On `SCALE_DOWN`, remove an idle server.

## 5. Public Functions and Time Complexity

Here `n` is the number of active servers (at most 6) and `q` is the number of queued requests.

| Function | Purpose | Complexity |
|---|---|---|
| `Loadbalancer(cm, rq)` | Stores the cluster and queue pointers, zeroes the counters | O(1) |
| `receiveRequest(req)` | Routes a new request, or queues it | O(n log n) |
| `tryAssign(req)` (private) | Builds the heap, picks the best server, assigns | O(n log n) |
| `drainQueue()` (private) | Re-routes waiting requests into free capacity | O(q · n log n) |
| `processTick()` | Ticks servers, collects completions, drains, scales | O(n + q · n log n) |
| `getTotalRequestsReceived()` / `getTotalRequestsProcessed()` | Counters | O(1) |

`ServerMaxHeap`

| Function | Purpose | Complexity |
|---|---|---|
| `insert(server)` | Adds a server and sifts it up | O(log n) |
| `extractMax()` | Removes and returns the best server (`nullptr` if empty) | O(log n) |
| `size()` / `empty()` | Heap size | O(1) |
| `siftup()` / `siftdown()` (private) | Restore the heap property | O(log n) |

## 6. Worked Example

Three servers, capacity 10: Server 1 has load 4, Server 2 has load 4, Server 3 has load 7.

- Free capacity is 6, 6 and 3.
- Servers 1 and 2 tie at 6, and Server 1 has the lower ID, so Server 1 is the root.
- The request goes to Server 1, which now has load 5. Next time Server 2 (free 6) is the root.

## 7. Testing

- The heap was tested on its own with a separate test file under `TESTS/` before it was used in the load balancer. The checks cover insertion order, the tie-break, draining the heap fully, and `extractMax()` on an empty heap.
- Routing scenarios to run through the whole system: basic routing, most-available-wins, tie-break, queue fallback when all servers are full, queue draining after a tick, and crash recovery (requests return to the queue front and are placed again).

## 8. Known Limitations

- **Not thread-safe.** The caller must hold the shared mutex (`sim_mutex` in `main.cpp`) around every call.
- **Rebuilding the heap per call** costs O(n log n) instead of O(log n). This is acceptable at 6 servers. For a large cluster, an indexed heap that supports priority updates would be the scalable alternative.
- **A new request can skip ahead of queued ones.** `receiveRequest` tries the servers first, even when requests are already waiting. If a server has room, the new request is placed before older queued requests. Strict FIFO would require enqueuing first and draining afterwards.
- **`GetRoot()` in `ServerMaxHeap` is a debugging helper** that prints to the console. It is only for tests.
- **Capacity counts request slots, not payload size.** Light, medium and heavy requests all use one slot. They differ only in how many ticks they need (1, 3 and 5).
- **The scaler decision uses the server snapshot taken at the start of the tick**, before the drain step.- `remaining_ticks`
- `arrival_time`

The queue stores these Request objects inside linked-list nodes. It does not independently generate request IDs or manage request processing time.

## 6. Example Execution Trace

Suppose requests 100, 101 and 102 are inserted.

| Operation | Queue (Front → Back) |
|---|---|
| Enqueue 100 | 100 |
| Enqueue 101 | 100 → 101 |
| Enqueue 102 | 100 → 101 → 102 |
| Dequeue | 101 → 102 |
| Enqueue front 99 | 99 → 101 → 102 |
| Peek | 99 → 101 → 102 |

The `peek()` operation does not remove the request or change the queue size.

## 7. Testing Performed

The following tests were performed:

- Normal enqueue and dequeue operations.
- FIFO ordering verification.
- Front insertion into an empty queue.
- Peek operation without changing queue size.
- Queue size tracking.
- Empty queue handling for `dequeue()` and `peek()`.
- Destructor behavior with remaining nodes.
- Independent compilation and complete project compilation.

**Result:** All reported queue tests passed, and the complete project compiled and ran successfully.
