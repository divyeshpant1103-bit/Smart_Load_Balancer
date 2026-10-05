#include "../include/failure_handler.hpp"
#include <iostream>

FailureHandler::FailureHandler(int id)
{
    serverId = id;
    serverActive = true;
}

void FailureHandler::simulateCrash()
{
    serverActive = false;

    std::cout << "Server " << serverId
              << " has failed.\n";
}

void FailureHandler::recoverServer()
{
    serverActive = true;

    std::cout << "Server " << serverId
              << " has been recovered.\n";
}

bool FailureHandler::isServerActive() const
{
    return serverActive;
}