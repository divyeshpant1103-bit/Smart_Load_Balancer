
#ifndef AUTO_SCALER_HPP
#define AUTO_SCALER_HPP
#include "server.hpp"
#include <vector>

enum class ScalingDecision
{
    SCALE_UP, // default 0 
    SCALE_DOWN, // default 1
    NO_ACTION // default 2
};

class autoScaler
{
private:
    int scaleUpThreshold; 
    int scaleDownThreshold;
    int minimumServers;
    int lowUtilizationStreak;
    int scalingStreakLimit;

public:
    autoScaler();

    ScalingDecision checkScaling(const std::vector<Server *> &servers,int queueSize);

    int getLowUtilizationStreak() const;
};

#endif
