#include <iostream>
#include <limits>
#include <vector>
#include "../include/common.hpp"
#include "../include/clusterManager.hpp"
#include "../include/RequestQueue.hpp"
#include "../include/load_balancer.hpp"
#include "../include/server.hpp"

void printDashboard(ClusterManager &cluster, RequestQueue &queue, Loadbalancer &lb)
{
    std::cout << "\n=========================================\n";
    std::cout << "         SYSTEM DASHBOARD SNAPSHOT       \n";
    std::cout << "=========================================\n";
    std::vector<Server *> activeServers = cluster.getActiveServers();
    std::cout << "Active Servers : " << activeServers.size() << " / " << SystemConfig::MAX_CLUSTER_SIZE << "\n";
    std::cout << "Queue Backlog  : " << queue.size() << " requests waiting\n";
    std::cout << "Total Received : " << lb.getTotalRequestsReceived() << "\n";
    std::cout << "Total Processed: " << lb.getTotalRequestsProcessed() << "\n\n";

    std::cout << "--- Server Status ---\n";
    if (activeServers.empty())
    {
        std::cout << "No active servers.\n";
    }
    else
    {
        for (Server *s : activeServers)
        {
            std::cout << "[Server " << s->getId() << "] Load: "
                      << s->getCurrentLoad() << "/" << s->getMaxCapacity()
                      << " | Status: " << (s->getStatus() == ServerStatus::ACTIVE ? "ACTIVE" : "INACTIVE") << "\n";
        }
    }
    std::cout << "=========================================\n\n";
}

// Advances the system by `ticks` time-steps.
void runTicks(Loadbalancer &lb, int ticks)
{
    std::cout << "[*] Processing " << ticks << " system tick" << (ticks == 1 ? "" : "s") << "...\n";
    for (int i = 0; i < ticks; i++)
    {
        lb.processTick();
    }
}

// Reads a positive integer; returns false (and clears the stream) on bad input.
bool readPositiveInt(const char *prompt, int &value)
{
    std::cout << prompt;
    if (!(std::cin >> value) || value <= 0)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please enter a positive whole number.\n";
        return false;
    }
    return true;
}

int main()
{
    ClusterManager cluster;
    RequestQueue queue;
    Loadbalancer lb(&cluster, &queue);

    std::cout << "Booting Smart Cloud Simulator...\n";
    cluster.addServer();
    cluster.addServer();

    int next_req_id = 100;
    int choice;

    while (true)
    {
        std::cout << "\n[ SIMULATOR CONTROL PANEL (MANUAL MODE) ]\n";
        std::cout << "1. Send a single LIGHT request (1 tick)\n";
        std::cout << "2. Send a single HEAVY request (5 ticks)\n";
        std::cout << "3. Send a BATCH of MEDIUM requests (3 ticks)\n";
        std::cout << "4. Process 1 Tick   (Clears LIGHT requests)\n";
        std::cout << "5. Process 3 Ticks  (Clears MEDIUM requests)\n";
        std::cout << "6. Process 5 Ticks  (Clears HEAVY requests)\n";
     
        std::cout << "8. Print System Dashboard\n";
        std::cout << "9. Exit\n";
        std::cout << "Enter choice: ";

        if (!(std::cin >> choice))
        {
            std::cin.clear();//clear buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//discard input
            continue;
        }

        if (choice == 9)
        {
            std::cout << "Shutting down simulator...\n";
            break;
        }

        int batchSize = 0;
        if (choice == 3)
        {
            if (!readPositiveInt("Enter batch size (e.g., 15): ", batchSize))
            {
                continue;
            }
        }

       

        switch (choice)
        {
        case 1:
        {
            Request req(next_req_id++, RequestType::LIGHT, 0);
            lb.receiveRequest(req);
            std::cout << "[+] Routed LIGHT Request #" << req.request_id << "\n";
            break;
        }
        case 2:
        {
            Request req(next_req_id++, RequestType::HEAVY, 0);
            lb.receiveRequest(req);
            std::cout << "[+] Routed HEAVY Request #" << req.request_id << "\n";
            break;
        }
        case 3:
        {
            for (int i = 0; i < batchSize; i++)
            {
                Request req(next_req_id++, RequestType::MEDIUM, 0);
                lb.receiveRequest(req);
            }
            std::cout << "[!] Batch of " << batchSize << " MEDIUM requests injected.\n";
            break;
        }
        case 4:
            runTicks(lb, 1);//in main
            break;
        case 5:
            runTicks(lb, 3);
            break;
        case 6:
            runTicks(lb, 5);
            break;
        case 8:
        {
            printDashboard(cluster, queue, lb);
            break;
        }
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}