#ifndef FAILURE_HANDLER_HPP
#define FAILURE_HANDLER_HPP

class FailureHandler
{
private:
    int serverId;
    bool serverActive;

public:
    FailureHandler(int id);

    void simulateCrash();
    void recoverServer();

    bool isServerActive() const;
};

#endif