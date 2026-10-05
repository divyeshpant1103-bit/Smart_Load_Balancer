#ifndef REPORT_GENERATOR_HPP
#define REPORT_GENERATOR_HPP

#include <vector>

class ReportGenerator
{
private:
    void merge(std::vector<double>& values, int left, int mid, int right);
    void mergeSort(std::vector<double>& values, int left, int right);

public:
    ReportGenerator();

    void generateReport(std::vector<double> utilization);
};

#endif