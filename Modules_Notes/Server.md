# Module Notes: Server & Core Infrastructure

**Owner:** Vaishnavi Garg
---

## 1. What this Module does

This module sets up the foundation of our simulator using two main files:
* **common.hpp:** Holds all shared settings and constants for the whole project (like maximum server capacity and max cluster size) so every file uses the same rules.

* **server.hpp:** Defines a server node. It handles incoming request, tracks current load, and remaining space so the load balancer knows where to send traffic.
---

## 2. Key Design Decisions

### common.hpp:
* **Decision:** Put all default settings inside a `SystemConfig` struct using `static constexpr`.
  **Why:** This calculates values at compile time for maximum speed and keeps configuration unified in one place. If we need to change cluster limits, we only change this one file.

### server.hpp:
* **Decision:** Deleted copy operations (`Server(const Server&) = delete`).
  **Why:** A server represents a unique machine that owns memory. Disabling copies prevents accidental duplicates, memory leaks, and crash bugs caused by double-deleting memory.

* **Decision:** Marked getter functions as `const` (like `getId() const`).
  **Why:** Tells the compiler these functions only read data without changing the server, allowing us to safely pass servers as read-only references.

---