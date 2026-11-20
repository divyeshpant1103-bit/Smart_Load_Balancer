# AutoScaler Module

## 1. Objective

To automatically increase or decrease the number of active servers
based on average server utilization and request queue size.

## 2. Overview

The AutoScaler module monitors the utilization of active servers
and decides whether additional servers are required or some servers
can be removed.

It supports three scaling decisions:

- SCALE_UP
- SCALE_DOWN
- NO_ACTION

## 3. Working

1. Receives the list of servers and current request queue size.
2. Checks the status of each server.
3. Calculates the average utilization of active servers.
4. If average utilization exceeds 75% and queue size is greater
   than 2, it returns SCALE_UP.
5. If average utilization is below 25% and active servers exceed
   the minimum server count, it tracks consecutive low-utilization
   checks.
6. After two consecutive low-utilization checks, it returns
   SCALE_DOWN.
7. Otherwise, it returns NO_ACTION.

## 4. Configuration Parameters

| Parameter | Value |
|---|---:|
| Initial server count | 3 |
| Scale-up threshold | 75% |
| Scale-down threshold | 25% |
| Scaling streak limit | 2 |
| Maximum cluster size | 6 |

## 5. Data Structures Used

| Data Structure | Purpose |
|---|---|
| Vector | Stores server pointers |
| Enumeration | Represents scaling decisions |
| Integer variables | Maintain thresholds and counters |

## 6. Important Functions

### autoScaler()

Initializes scaling thresholds, minimum server count,
and utilization streak counter.

### checkScaling()

Calculates average utilization and determines the
appropriate scaling decision.

### getLowUtilizationStreak()

Returns the current consecutive low-utilization count.

## 7. Test Cases

| Test ID | Description |
|---|---|
| AS-01 | Scale-up condition |
| AS-02 | Scale-down condition |
| AS-03 | No scaling required |
| AS-04 | Minimum server limit |
| AS-05 | Consecutive low-utilization checks |

## 8. Algorithm

1. Check whether the server list is empty.
2. Count active servers and calculate their total utilization.
3. Calculate average utilization.
4. Check the scale-up condition.
5. Check the scale-down condition.
6. Return the appropriate scaling decision.

## 9. Complexity Analysis

- Time Complexity: O(n)
- Auxiliary Space Complexity: O(1)

Here, n represents the number of servers.

## 10. Integration

The AutoScaler module works with the Loadbalancer and
ClusterManager modules.

The Loadbalancer passes the active server list and queue size
to AutoScaler. Based on the returned decision, Loadbalancer
uses ClusterManager to add or remove a server.

## 11. Conclusion

The AutoScaler module helps maintain efficient server resource
utilization by dynamically adjusting the number of active servers
according to workload conditions.