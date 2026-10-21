#include "common.hpp"
#include "auto_scaler.hpp"
class ClusterManager;
class RequestQueue;
class Loadbalancer
{

private:
    ClusterManager *cluster_manager;
    RequestQueue *request_queue;
    autoScaler scaler; // Add an instance of autoScaler
    int total_requests_processed;
    int total_requests_received;

    
    bool tryAssign(const Request &req);
    void drainQueue();

public:
    Loadbalancer(ClusterManager *cm, RequestQueue *rq);
    ~Loadbalancer();

    void receiveRequest(const Request &req);
    void processTick();
    int getTotalRequestsProcessed();
    int getTotalRequestsReceived();
};