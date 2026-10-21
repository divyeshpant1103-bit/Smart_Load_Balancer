## 1. What This Module Does

The Request & Queue Management module is responsible for creating and managing client requests in the Smart Cloud Load Balancer.

The Request class stores the unique request ID and the timestamp at which the request is created.

The RequestQueue class manages requests using a linked-list-based FIFO queue. It supports adding requests at the back, removing requests from the front, inserting recovered requests at the front, checking the front request without removing it, and tracking the current number of requests.

The module also handles empty queues and releases dynamically allocated queue nodes through its destructor.

## 2. Key Design Decisions (and Why)

### 2.1 Linked List instead of Array

A linked list is used to implement the RequestQueue instead of a fixed-size array.

Why:
- The queue can grow dynamically as requests arrive.
- There is no fixed maximum queue capacity.
- Enqueue and dequeue can be performed in O(1) time when front and back pointers are maintained.

Why not an array:
- A fixed-size array has a predefined capacity.
- Managing elements when the queue becomes full or requires resizing adds complexity.

### 2.2 Maintaining Both Front and Back Pointers

Both front and back pointers are maintained.

Why:
- front allows O(1) dequeue.
- back allows O(1) enqueue.

If only front were maintained, finding the last node would require traversing the linked list, making enqueue O(n).

### 2.3 Maintaining a Count Variable

A count variable stores the current number of requests.

Why:
- size() can return the queue size in O(1).

Why not calculate the size by traversal:
- Traversing the entire linked list every time would make size() O(n).



