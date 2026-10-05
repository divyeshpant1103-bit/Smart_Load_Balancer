#include "../include/RequestQueue.hpp"

RequestQueue::RequestQueue()
{
    front = nullptr;
    back = nullptr;
    count=0;
}
void RequestQueue::enqueue(Request request)
{
    Node* newNode = new Node{request, nullptr};

    if (front == nullptr)
    {
        front = newNode;
        back = newNode;
    }
    else
    {
        back->next = newNode;
        back = newNode;
    }

    count++;
}