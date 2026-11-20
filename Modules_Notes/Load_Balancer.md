# Request & Queue Management Module


## 1. What This Module Does

The Request & Queue Management module manages incoming requests in the Smart Load Balancer project. It stores requests in a queue and processes them in First-In-First-Out (FIFO) order.

It uses a custom singly linked list to dynamically manage requests. It also supports inserting requests at the front, viewing the next request, checking queue size, and removing completed requests.

The module uses the shared `Request` structure defined in `common.hpp`.

## 2. Key Design Decisions

- **Singly Linked List:** Used instead of an array because it supports dynamic memory allocation without a fixed queue size.
- **Front and Back Pointers:** `front` points to the first request, while `back` points to the last request. This makes insertion at the rear efficient.
- **Count Variable:** Maintains the number of requests without traversing the entire list.
- **FIFO Principle:** Requests are normally processed in the order they arrive.
- **Dynamic Memory Allocation:** Nodes are created using `new` and released using `delete`.
- **Front Insertion:** `enqueue_front()` allows a request to be inserted at the beginning of the queue.

## 3. Public Functions and Time Complexity

| Function | Purpose | Complexity |
|---|---|---|
| `RequestQueue()` | Initializes an empty queue | O(1) |
| `enqueue()` | Inserts a request at the rear | O(1) |
| `dequeue()` | Removes and returns the front request | O(1) |
| `enqueue_front()` | Inserts a request at the front | O(1) |
| `peek()` | Returns the front request without removing it | O(1) |
| `size()` | Returns the number of requests | O(1) |
| `~RequestQueue()` | Frees all dynamically allocated nodes | O(n) |

## 4. Known Limitations

- Empty queue operations return a dummy request with `request_id = -1`.
- The caller must check the request ID to identify an invalid or empty result.
- Dynamic memory allocation requires proper memory management.
- The queue does not independently validate request IDs.
- The queue is not thread-safe; synchronization is required if multiple threads access it concurrently.
- Copying a `RequestQueue` object is unsafe with the current raw-pointer implementation and should be prevented or properly implemented.

## 5. Important Design Notes

The shared `Request` structure contains:

- `request_id`
- `type`
- `remaining_ticks`
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