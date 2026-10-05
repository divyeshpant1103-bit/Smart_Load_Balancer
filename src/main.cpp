#include "../include/failure_handler.hpp"
#include "../include/auto_scaler.hpp"
#include "../include/report_generator.hpp"
#include <vector>
#include <iostream>

int main()
{
    // FailureHandler test
    FailureHandler handler(1);

    std::cout << "Initial server status: "
              << handler.isServerActive() << "\n";

    handler.simulateCrash();

    std::cout << "Server status after crash: "
              << handler.isServerActive() << "\n";

    handler.recoverServer();

    std::cout << "Server status after recovery: "
              << handler.isServerActive() << "\n";


    // AutoScaler test
    AutoScaler scaler;

    std::cout << "\nAutoScaler Tests:\n";

    scaler.checkScaling(80.0, 0, 3);  // High utilization
    scaler.checkScaling(20.0, 0, 3);  // Low utilization
    scaler.checkScaling(50.0, 1, 3);  // Normal load

    // ReportGenerator test
    ReportGenerator report;

    std::vector<double> utilization = {45.0, 80.0, 30.0, 95.0, 60.0};

    report.generateReport(utilization);
    return 0;
}