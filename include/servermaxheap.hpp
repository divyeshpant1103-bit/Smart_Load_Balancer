#ifndef SERVERMAXHEAP_HPP // include guard to prevent multiple inclusions of the header file
#define SERVERMAXHEAP_HPP // define the include guard
#include <vector>         //include the vector header for using std::vector
#include "server.hpp"     //include the server header for using the Server class
class ServerMaxHeap
{
private:
    std::vector<Server *> heap;
    // define priority logic
    bool isHigherPriority(Server *a, Server *b)
    {
        int availableA = a->getMaxCapacity() - a->getCurrentLoad();
        int availableB = b->getMaxCapacity() - b->getCurrentLoad();
        if (availableA > availableB)
        {
            return true;
        }
        else if (availableA == availableB)
        { // tie-breaker based on server id
            return a->getId() < b->getId();
        }
        else
        {
            return false;
        }
    }
    // logic to sift up and sift down the heap to maintain the max-heap property
    void siftup(int index)
    {
        while (index > 0)
        {
            int parent = (index - 1) / 2;
            if (isHigherPriority(heap[index], heap[parent]))
            {
                std::swap(heap[index], heap[parent]);
                index = parent;
            }
            else
            {
                break;
            }
        }
    }

    void siftdown(int index)
    {
        int size = heap.size();
        while (index < size)
        {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;
            if (left < size && isHigherPriority(heap[left], heap[largest]))
            {
                largest = left;
            }
            if (right < size && isHigherPriority(heap[right], heap[largest]))
            {
                largest = right;
            }
            if (largest != index)
            {
                std::swap(heap[index], heap[largest]);
                index = largest;
            }
            else
            {
                break;
            }
        }
    }
    public:
    void insert(Server *server){
       heap.push_back(server);
        siftup(heap.size() - 1);
    }
    int size(){
        return heap.size();
    }
    bool empty(){
        return heap.empty();
    }
    Server *extractMax(){
        if(heap.empty()){
            return nullptr;
        }else{
            Server *maxServer = heap[0];
            heap[0] = heap.back();
            heap.pop_back();
            siftdown(0);
            return maxServer;
        }
    }
    //test Case Purpose
    void GetRoot(){
        if(!heap.empty()){
            Server *maxServer = heap[0];
            std::cout << "Root Server ID: " << maxServer->getId() << std::endl;
        }else{
            std::cout << "Heap is empty." << std::endl;
        }
    }
};
#endif // end of the include guard