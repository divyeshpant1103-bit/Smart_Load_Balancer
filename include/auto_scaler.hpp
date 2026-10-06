#ifndef AUTO_SCALER_HPP
#define AUTO_SCALER_HPP

#include "server.hpp"
#include <vector>

enum class ScalingDecision
{
    SCALE_UP,
    SCALE_DOWN,
    NO_ACTION
};

class autoScaler{
    private:
        double scaleUpThreshold;
        double scaleDownThreshold;
        int minimumServers;
        int lowUtilizationStreak;
        int scalingStreakLimit;
    public:
        autoScaler();
        ScalingDecision checkScaling(const std::vector<Server*>& servers, int queueSize);
        int getLowUtilizationStreak() const;
};

#endif