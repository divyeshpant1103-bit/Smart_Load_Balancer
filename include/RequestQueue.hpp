#ifndef REQUEST_QUEUE_HPP
#define REQUEST_QUEUE_HPP

#include "Request.hpp"

class RequestQueue
{
private:
    struct Node
    {
        Request request;
        Node* next;
    };
    Node* front;
    Node* back;
    int count;

public:
    RequestQueue();
    ~RequestQueue();
    void enqueue(Request request);
    Request dequeue();
    void enqueue_front(Request request);
    int size();
    Request peek();
};
#endif