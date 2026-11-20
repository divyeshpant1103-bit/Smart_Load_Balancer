# FailureHandler Module

## 1. Objective

To handle server crashes and recovery by updating server status and transferring unfinished requests back to the request queue.

## 2. Overview

The FailureHandler module manages the failure and recovery of individual servers.

It uses ClusterManager to locate servers, Server to extract unfinished requests, and RequestQueue to store requests recovered from a failed server.

## 3. Working

### Server Crash (`simulateCrash()`)

1. Finds the server using its ID through ClusterManager.
2. Checks whether the server exists.
3. Checks whether the server is already inactive.
4. Changes the server status to `INACTIVE`.
5. Extracts unfinished requests into a temporary vector.
6. Inserts the requests at the front of RequestQueue in their original order.
7. Displays the number of requeued requests.

### Server Recovery (`recoverServer()`)

1. Finds the server using its ID.
2. Checks whether the server exists.
3. Checks whether the server is already active.
4. Changes the server status to `ACTIVE`.
5. Updates the local server state.

## 4. Data Members

| Data Member | Purpose |
|---|---|
| `serverId` | Stores the ID of the server being handled |
| `serverActive` | Tracks the server's active state |
| `cluster` | Reference to ClusterManager |
| `requestQueue` | Reference to RequestQueue |

## 5. Important Functions

### FailureHandler()

Initializes the server ID, sets the initial active state to `true`, and stores references to ClusterManager and RequestQueue.

### simulateCrash()

Marks the specified server as inactive, extracts unfinished requests, and requeues them while preserving their original order.

### recoverServer()

Changes the server status back to `ACTIVE` and updates the local active state.

### isServerActive()

Returns the current value of the `serverActive` variable.

## 6. Data Structures Used

| Data Structure | Purpose |
|---|---|
| Vector | Temporarily stores unfinished requests |
| Linked List | Used internally by RequestQueue |
| References | Provide access to ClusterManager and RequestQueue |

## 7. Test Cases

| Test ID | Description | Expected Result |
|---|---|---|
| FH-01 | Server crash test | Server becomes inactive |
| FH-02 | Server recovery test | Server becomes active again |
| FH-03 | Request requeue test | Unfinished requests are requeued in original order |

**Test Result:** All three FailureHandler tests passed.

## 8. Algorithm

### Crash Algorithm

1. Locate the server.
2. Check whether it exists and is not already inactive.
3. Mark it as inactive.
4. Extract unfinished requests.
5. Requeue the requests in their original order.

### Recovery Algorithm

1. Locate the server.
2. Check whether it exists and is not already active.
3. Set its status to active.
4. Update the local active state.

## 9. Complexity Analysis

- Request requeueing: O(n), assuming constant-time insertion at the front of the queue.
- Overall complexity depends on the implementation of server lookup and unfinished-request extraction.

Here, n represents the number of unfinished requests.

## 10. Integration

The FailureHandler module interacts with:

- **ClusterManager:** Locates the server using its ID.
- **Server:** Updates server status and extracts unfinished requests.
- **RequestQueue:** Stores the unfinished requests for future processing.

## 11. Conclusion

The FailureHandler module handles server failures and recovery while ensuring that unfinished requests are returned to the request queue without changing their original order.