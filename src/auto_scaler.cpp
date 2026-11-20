#include "../include/auto_scaler.hpp"
#include <iostream>

autoScaler::autoScaler(){
    scaleUpThreshold = SystemConfig::SCALE_UP_THRESHOLD; 
    scaleDownThreshold = SystemConfig::SCALE_DOWN_THRESHOLD; 
    minimumServers = SystemConfig::INITIAL_SERVER_COUNT;     
    lowUtilizationStreak = 0;
    scalingStreakLimit = SystemConfig::SCALING_STREAK_LIMIT; 
}

ScalingDecision autoScaler :: checkScaling(const std::vector<Server*>& servers, int queueSize){
    if(servers.empty()){
        return ScalingDecision::SCALE_UP; 
    }
    double totalUtilization = 0.0;
    int activeServers = 0;
    for(Server* server : servers){
        if(server == nullptr) continue;
        if(server->getStatus() == ServerStatus::ACTIVE){
            totalUtilization += server->getUtilizationPercentage();
            activeServers++;
        }
    }
    if(activeServers == 0){
        lowUtilizationStreak = 0;
        return ScalingDecision::SCALE_UP;
    }
    double averageUtilization = totalUtilization / activeServers;
    
    if(averageUtilization > scaleUpThreshold && queueSize>2){
        lowUtilizationStreak = 0; 
        std::cout << "autoScaler: Scale Up Required.\n";
        return ScalingDecision::SCALE_UP;
    }
    if(averageUtilization < scaleDownThreshold && activeServers > minimumServers){
        lowUtilizationStreak++;
        if(lowUtilizationStreak >= scalingStreakLimit){
            std::cout << "autoScaler: Scale Down Required.\n";
            lowUtilizationStreak = 0; 
            return ScalingDecision::SCALE_DOWN;
        }
    } else {
        lowUtilizationStreak = 0; 
    }
    std::cout << "autoScaler: No Scaling Action Required.\n";
    return ScalingDecision::NO_ACTION;
}

int autoScaler::getLowUtilizationStreak() const{
    return lowUtilizationStreak;
}
