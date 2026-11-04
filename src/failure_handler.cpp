
#include "../include/failure_handler.hpp"
#include <iostream>
#include <vector>

FailureHandler::FailureHandler(
    int id,
    ClusterManager& manager,
    RequestQueue& queue
)
    : serverId(id),
      serverActive(true),
      cluster(manager),
      requestQueue(queue)
{
}

void FailureHandler::simulateCrash()
{
    Server* server = cluster.findServer(serverId);

    if (server == nullptr)
    {
        std::cout << "Server " << serverId << " not found.\n";
        return;
    }

    if (server->getStatus() == ServerStatus::INACTIVE)
    {
        serverActive = false;
        std::cout << "Server " << serverId
                  << " is already inactive.\n";
        return;
    }

    // Mark server as inactive
    server->setStatus(ServerStatus::INACTIVE);
    serverActive = false;

    // Extract unfinished requests
    std::vector<Request> unfinishedRequests;

    server->extractUnfinishedRequests(unfinishedRequests);

    // Requeue requests while preserving their original order
    for (auto it = unfinishedRequests.rbegin();
         it != unfinishedRequests.rend(); ++it)
    {
        requestQueue.enqueue_front(*it);
    }

    std::cout << "Server " << serverId
              << " has failed.\n";

    std::cout << unfinishedRequests.size()
              << " unfinished requests requeued.\n";
}

void FailureHandler::recoverServer()
{
    Server* server = cluster.findServer(serverId);

    if (server == nullptr)
    {
        std::cout << "Server " << serverId << " not found.\n";
        return;
    }

    if (server->getStatus() == ServerStatus::ACTIVE)
    {
        serverActive = true;
        std::cout << "Server " << serverId
                  << " is already active.\n";
        return;
    }

    server->setStatus(ServerStatus::ACTIVE);
    serverActive = true;

    std::cout << "Server " << serverId
              << " has been recovered.\n";
}

bool FailureHandler::isServerActive() const
{
    return serverActive;
}
