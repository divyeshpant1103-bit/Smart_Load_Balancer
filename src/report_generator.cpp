#include "../include/report_generator.hpp"
#include <iostream>

ReportGenerator::ReportGenerator()
{
}

void ReportGenerator::merge(std::vector<double>& values,
                            int left,
                            int mid,
                            int right)
{
    std::vector<double> leftPart;
    std::vector<double> rightPart;

    for (int i = left; i <= mid; i++)
        leftPart.push_back(values[i]);

    for (int i = mid + 1; i <= right; i++)
        rightPart.push_back(values[i]);

    int i = 0;
    int j = 0;
    int k = left;

    // Descending order
    while (i < leftPart.size() && j < rightPart.size())
    {
        if (leftPart[i] > rightPart[j])
            values[k++] = leftPart[i++];
        else
            values[k++] = rightPart[j++];
    }

    while (i < leftPart.size())
        values[k++] = leftPart[i++];

    while (j < rightPart.size())
        values[k++] = rightPart[j++];
}

void ReportGenerator::mergeSort(std::vector<double>& values,
                                int left,
                                int right)
{
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    mergeSort(values, left, mid);
    mergeSort(values, mid + 1, right);

    merge(values, left, mid, right);
}

void ReportGenerator::generateReport(std::vector<double> utilization)
{
    if (utilization.empty())
    {
        std::cout << "No server data available.\n";
        return;
    }

    mergeSort(utilization, 0, utilization.size() - 1);

    std::cout << "\nServer Utilization Report:\n";

    for (double value : utilization)
    {
        std::cout << value << "%\n";
    }
}