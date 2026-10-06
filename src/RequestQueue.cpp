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
Request RequestQueue::dequeue()
{
    if (front == nullptr)
    {
        return Request(-1);
    }
    Request request = front->request;
    Node* temp = front;

    front = front->next;

    if (front == nullptr)
    {
        back = nullptr;
    }

    delete temp;
    count--;

    return request;
}


int RequestQueue::size()
{
    return count;
}