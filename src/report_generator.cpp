
#include "../include/report_generator.hpp"

ReportGenerator::ReportGenerator()
{
}

void ReportGenerator::merge(
    std::vector<ServerReport>& values,
    int left,
    int mid,
    int right)
{
    std::vector<ServerReport> leftPart;
    std::vector<ServerReport> rightPart;

    for (int i = left; i <= mid; i++)
        leftPart.push_back(values[i]);

    for (int i = mid + 1; i <= right; i++)
        rightPart.push_back(values[i]);

    int i = 0;
    int j = 0;
    int k = left;

    // Descending order
    while (i < static_cast<int>(leftPart.size()) &&
           j < static_cast<int>(rightPart.size()))
    {
        if (leftPart[i].utilization >= rightPart[j].utilization)
            values[k++] = leftPart[i++];
        else
            values[k++] = rightPart[j++];
    }

    while (i < static_cast<int>(leftPart.size()))
        values[k++] = leftPart[i++];

    while (j < static_cast<int>(rightPart.size()))
        values[k++] = rightPart[j++];
}

void ReportGenerator::mergeSort(
    std::vector<ServerReport>& values,
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

std::vector<ServerReport> ReportGenerator::generateReport(
    const std::vector<Server*>& servers)
{
    std::vector<ServerReport> reportData;

    for (Server* server : servers)
    {
        if (server == nullptr ||
            server->getStatus() != ServerStatus::ACTIVE)
            continue;

        ServerReport data;
        data.serverId = server->getId();
        data.utilization = server->getUtilizationPercentage();

        reportData.push_back(data);
    }

    if (!reportData.empty())
    {
        mergeSort(
            reportData,
            0,
            static_cast<int>(reportData.size()) - 1
        );
    }

    return reportData;
}
