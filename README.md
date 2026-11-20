# SMART CLOUD INFRASTRUCTURE AND LOAD BALANCER SIMULATOR

A console-based C++ simulator which models the way a cloud infrastructure distributes, queues, scales and recovers from failures when dealing with a continuously varying stream of incoming requests.
The entire program being based on core Data structures, Algorithms and Object-oriented programming, without using a real cloud platform.

---

## Problem Statement

Modern applications see unpredictable and constantly changing traffic. A small number of servers can get overloaded and cause delays, while running too many idle servers wastes resources. Real cloud providers solve this with load balancing and auto-scaling but their internals are complex and hidden from students learning DSA.

---

## Objective

This project builds a simplified simulation in C++ that models:

* Routing incoming requests to the least-loaded available server.
* Queuing requests when all servers are at capacity.
* Dynamically scaling the number of active servers up or down based on load.
* Detecting a simulated server failure and recovering its in-flight requests without data loss.
* Producing a ranked utilization report across the cluster.

The goal is to demonstrate how linked lists, queues, heaps, sorting algorithms and OOP concepts map directly onto real distributed-systems problems.

---

## Team Members

**Team Name:** 4Bits
**Team ID:** DSCPP-III-2026-T174
**Institution:** Graphic Era University, Dehradun

| Role | Name | Enrollment No. |
| :--- | :--- | :--- |
| **Team Lead** | Sarthak Jaiswal | 2510011427 |
| **Member 1** | Vaishnavi Garg | 2510012701 |
| **Member 2** | Divyesh Pant | 2510011239 |
| **Member 3** | Hardik Aggarwal | 2510011450 |

---

## Technologies and Tools Used

* **Language:** C++
* **Editor/ IDE:** VS Code
* **Compiler:** Any standard C++ compiler (g++ / MSVC / clang++), C++11 or later
* **Concurrency:** std::thread, std::mutex for background request processing
* **Core DSA concepts:** Doubly Linked List, custom Queue (deque-like), Max-Heap, Merge Sort, Threshold-based control logic
* **Build tool:** Makefile / CMake
* **Version control:** Git & Github

---

## Major Features/ Modules

| Module | Responsibility | Core Technique |
| :--- | :--- | :--- |
| **`Server`** | Holds requests assigned to one machine; tracks capacity, load, and active status. | Encapsulation & Dynamic Queuing |
| **`ClusterManager`** | Owns the set of active servers; supports adding/removing servers | Doubly Linked List |
| **`RequestQueue`** | Holds requests waiting for free capacity; supports priority re-insertion for recovered requests. | Queue with front-insertion |
| **`LoadBalancer`** | Selects the best available server for each request based on real-time headroom. | Max-Heap |
| **`AutoScaler`** | Grows or shrinks the cluster based on average utilization and queue backlog, with hysteresis to prevent thrashing. | Threshold-based control logic |
| **`FailureHandler`** | Simulates a server crash and re-queues its in-flight requests with priority. | Queue re-insertion |
| **`ReportGenerator`** | Produces a ranked, sorted utilization report across the cluster. | Merge Sort |
| **`Background Processor`** | Completes requests and re-checks scaling conditions on a separate thread, synchronized safely with the main menu thread. | Multithreading + Mutex |

---

## Current Project Structure

```text
Smart_Load_Balancer/
├── include/
│   └── server.hpp          
├── Modules_Notes/
│   └── Load_Balancer.md
├── src/
│   └── main.cpp
├── .gitignore
├── Makefile
└── README.md
```
---

## Getting started

### Prerequisites
* A C++ compiler supporting C++17 or later (g++, clang++, or MSVC)
* make
* Git (to clone the repository)

### Build and Run Instructions

```bash
git clone https://github.com/divyeshpant1103-bit/Smart_Load_Balancer.git
cd Smart_Load_Balancer
make
./cloud_sim
```

**Note:** The project is currently in active module development phase. It will be fully executable upon upcoming feature merges.

---

## Current Project Status

**Phase:** Design & core module implementation (In Progress)

| Milestone | Status |
| :--- | :---: |
| **Requirement analysis & system design** | Complete |
| **Core DSA & OOP class implementation (Server, ClusterManager)** | In progress |
| **Request handling & load balancing (Queue, max-heap routing)** | In progress |
| **Auto-scaling & failure handling** | In progress |
| **Background processing & thread synchronization** | Not started |
| **Testing (normal / spike / queue-buildup scenarios)** | Not started |
| **Final report & demonstration** | Not started |

---