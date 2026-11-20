# ReportGenerator Module

## 1. Objective

To generate server utilization reports by collecting active server details and sorting them in descending order of utilization using the Merge Sort algorithm.

## 2. Overview

The ReportGenerator module collects utilization information from active servers.

It stores each server's ID and utilization percentage in a `ServerReport` structure and sorts the collected data from highest to lowest utilization.

The module returns the sorted report data instead of displaying it directly.

## 3. Working

1. Receives a vector of server pointers.
2. Checks each server pointer.
3. Skips null pointers and servers that are not ACTIVE.
4. Collects the server ID and utilization percentage.
5. Stores the information in a vector of `ServerReport` objects.
6. Sorts the data in descending order using Merge Sort.
7. Returns the sorted report vector.

## 4. Data Structures Used

| Data Structure | Purpose |
|---|---|
| Vector | Stores server pointers and report records |
| Structure | Stores server ID and utilization percentage |

### ServerReport Structure

Contains two members:

- `serverId`: Stores the server ID.
- `utilization`: Stores the server utilization percentage.

## 5. Important Functions

### ReportGenerator()

Default constructor of the ReportGenerator class.

### generateReport()

Collects information from active servers, creates report records, sorts them, and returns the sorted vector.

### mergeSort()

Recursively divides the report data into smaller parts and sorts them.

### merge()

Combines two sorted parts into one sorted section in descending order of utilization.

## 6. Algorithm Used: Merge Sort

Merge Sort follows the Divide and Conquer approach.

1. Divide the report data into two halves.
2. Recursively sort both halves.
3. Merge the sorted halves.
4. Arrange the records in descending order of utilization.

## 7. Test Cases

| Test ID | Description |
|---|---|
| RG-01 | Verify descending utilization order |
| RG-02 | Verify empty input |
| RG-03 | Verify single server input |
| RG-04 | Verify inactive server filtering |
| RG-05 | Verify equal utilization values |

**Test Result:** All five ReportGenerator tests passed.

## 8. Complexity Analysis

- **Time Complexity:** O(n log n)
- **Auxiliary Space Complexity:** O(n)

Here, n represents the number of active servers included in the report.

## 9. Integration

The ReportGenerator module interacts with:

- **Server:** Retrieves server IDs, status, and utilization percentages.
- **ClusterManager:** Supplies the active server list through `getActiveServers()`.
- **Main Module:** Can receive the sorted report data and display it.

## 10. Conclusion

The ReportGenerator module provides an organized view of server utilization by collecting active server information and sorting it using Merge Sort. This helps identify servers with higher utilization.