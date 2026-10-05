#include "../include/failure_handler.hpp"
#include <iostream>

int main()
{
    FailureHandler handler(1);

    std::cout << "Initial server status: "
              << handler.isServerActive() << "\n";

    handler.simulateCrash();

    std::cout << "Server status after crash: "
              << handler.isServerActive() << "\n";

    handler.recoverServer();

    std::cout << "Server status after recovery: "
              << handler.isServerActive() << "\n";

    return 0;
}