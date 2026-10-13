#ifndef LOAD_BALANCER_HPP
#define LOAD_BALANCER_HPP
#include "common.hpp"

class ClusterManager;
class RequestQueue;

class Loadbalancer{
    
    private:
        ClusterManager* cluster_manager;
        RequestQueue* request_queue;
        int total_requests_processed;
        int total_requests_received;

    public:
        Loadbalancer(ClusterManager* cm, RequestQueue* rq);
        ~Loadbalancer();

        void receiveRequest(const Request& req);
        void processTick();
        int getTotalRequestsProcessed();
        int getTotalRequestsReceived();
};