# Module Notes: Server & Core Infrastructure

**Owner:** Vaishnavi Garg
---

## 1. What this Module does

This module sets up the foundation of our simulator using two main files:
* **common.hpp:** Holds all shared settings and constants for the whole project (like maximum server capacity and max cluster size) so every file uses the same rules.

* **server.hpp:** Declares the `Server` class structure, node pointers for manual request tracking, and public method interfaces.

* **server.cpp:** Contains the actual execution logic for server operations—handling request assignments, progressing tasks per simulation tick, calculating load stats, and safely freeing memory.

---

## 2. Key Design Decisions

### common.hpp:
* **Decision:** Defined constants using `static const int` inside `SystemConfig`.
  **Why:** Keeps system limits grouped in one place without breaking C++ compilation rules across multiple `.cpp` files.


### server.hpp & server.cpp:
* **Decision:** Deleted copy operations (`Server(const Server&) = delete`).
  **Why:** A server represents a unique machine that owns memory. Disabling copies prevents accidental duplicates, memory leaks, and crash bugs caused by double-deleting memory.

* **Decision:** Separated memory cleanup into a dedicated `clearQueue()` helper function called by `~Server()`.
  **Why:** Allows runtime flushing and server resets without destroying the actual `Server` object.

* **Decision:** Used `static_cast<double>` in load calculation.  
  **Why:** Prevents integer division truncation from evaluating load percentage to zero.

---

## 3. Functions

| Function | What it does | Complexity |
| :--- | :--- | :--- |
| `SystemConfig` | Stores globally shared constants like capacity limits and thresholds. | O(1) |
| `Server(id, capacity)` | Creates a new server with a unique ID and capacity limit. | O(1) |
| `~Server()` | Cleans up allocated server memory by calling `clearQueue()`. | O(N) |
| `clearQueue()` | Deallocates all `RequestNode` pointers in the queue and resets load to 0. | O(N) |
| `getId()` | Returns the server's unique ID. | O(1) |
| `getCapacity()` | Returns the total request capacity of the server. | O(1) |
| `assignRequest(req)` | Adds a request node to the queue if capacity is available. | O(1) |

---