#include "../include/auto_scaler.hpp"
#include <iostream>

AutoScaler::AutoScaler()
{
    scaleUpThreshold = 75.0;
    scaleDownThreshold = 25.0;
    minimumServers = 2;
}

void AutoScaler::checkScaling(double averageUtilization,
                              int queueSize,
                              int activeServers)
{
    if (averageUtilization > scaleUpThreshold || queueSize > 2)
    {
        std::cout << "AutoScaler: Scale UP required.\n";
    }
    else if (averageUtilization < scaleDownThreshold &&
             activeServers > minimumServers)
    {
        std::cout << "AutoScaler: Scale DOWN required.\n";
    }
    else
    {
        std::cout << "AutoScaler: No scaling required.\n";
    }
}