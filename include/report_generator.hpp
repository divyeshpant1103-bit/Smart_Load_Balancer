
#ifndef REPORT_GENERATOR_HPP
#define REPORT_GENERATOR_HPP

#include <vector>
#include "server.hpp"

struct ServerReport
{
    int serverId;
    double utilization;
};

class ReportGenerator
{
private:
    void merge(std::vector<ServerReport>& values,
               int left, int mid, int right);

    void mergeSort(std::vector<ServerReport>& values,
                   int left, int right);

public:
    ReportGenerator();

    std::vector<ServerReport> generateReport(
        const std::vector<Server*>& servers
    );
};

#endif
