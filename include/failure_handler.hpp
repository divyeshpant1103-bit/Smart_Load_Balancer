
#ifndef FAILURE_HANDLER_HPP
#define FAILURE_HANDLER_HPP

#include "clusterManager.hpp"
#include "RequestQueue.hpp"
#include <vector>

class FailureHandler
{
private:
    int serverId;
    bool serverActive;

    ClusterManager& cluster;
    RequestQueue& requestQueue;

public:
    FailureHandler(
        int id,
        ClusterManager& manager,
        RequestQueue& queue
    );

    void simulateCrash();
    void recoverServer();

    bool isServerActive() const;
};

#endif
