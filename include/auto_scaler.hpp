#ifndef AUTO_SCALER_HPP
#define AUTO_SCALER_HPP

class AutoScaler
{
private:
    double scaleUpThreshold;
    double scaleDownThreshold;
    int minimumServers;

public:
    AutoScaler();

    void checkScaling(double averageUtilization, int queueSize, int activeServers);
};

#endif