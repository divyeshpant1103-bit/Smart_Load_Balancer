#include "load_balancer.hpp"
#include "clusterManager.hpp"
#include "RequestQueue.hpp"
#include "servermaxheap.hpp"
#include "common.hpp"
#include "auto_scaler.hpp"
#include "server.hpp"
#include <vector>

Loadbalancer::Loadbalancer(ClusterManager *cm, RequestQueue *rq)
    : cluster_manager(cm),
      request_queue(rq),
      total_requests_processed(0),
      total_requests_received(0) {}

Loadbalancer::~Loadbalancer() {}

bool Loadbalancer::tryAssign(const Request &req)
{
    std::vector<Server *> servers = cluster_manager->getActiveServers();
    ServerMaxHeap heap;
    for (Server *s : servers)
    {
        if (s->getCurrentLoad() < s->getMaxCapacity())
            heap.insert(s);
    }
    Server *best = heap.extractMax();
    if (best == nullptr)
        return false;
    return best->assignRequest(req);
}

void Loadbalancer::receiveRequest(const Request &req)
{
    total_requests_received++;
    if (!tryAssign(req))
        request_queue->enqueue(req);
}

void Loadbalancer::drainQueue()
{
    while (request_queue->size() > 0)
    {
        Request r = request_queue->dequeue();
        if (!tryAssign(r))
        {
            request_queue->enqueue_front(r);
            break;
        }
    }
}

void Loadbalancer::processTick()
{
    // 1. Fetch current active servers
    std::vector<Server *> servers = cluster_manager->getActiveServers();

    // 2. Process work on each server
    for (Server *s : servers)
    {
        s->processTick();
        Request done; // Using the default constructor we added earlier!
        while (s->popCompletedRequest(done))
        {
            total_requests_processed++;
        }
    }

    //  Try to route any waiting requests into the newly freed capacity
    drainQueue();

    //  Check Auto-Scaling conditions using your module
    int currentQueueSize = request_queue->size();
    ScalingDecision decision = scaler.checkScaling(servers, currentQueueSize);

    if (decision == ScalingDecision::SCALE_UP)
    {
        cluster_manager->addServer();
        // Immediately try to route waiting requests into the brand new server
        drainQueue();
    }
    else if (decision == ScalingDecision::SCALE_DOWN)
    {
        cluster_manager->removeIdleServer();
    }
}

int Loadbalancer::getTotalRequestsProcessed() { return total_requests_processed; }
int Loadbalancer::getTotalRequestsReceived() { return total_requests_received; }