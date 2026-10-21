#include <iostream>
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

int main()
{
    // 1. Initialize Core System
    ClusterManager cluster;
    RequestQueue queue;
    Loadbalancer lb(&cluster, &queue);

    std::cout << "Booting Smart Cloud Simulator...\n";
    cluster.addServer();
    cluster.addServer();

    int next_req_id = 100;
    int choice;

    // 2. Main Menu Loop (Manual Mode)
    while (true)
    {
        std::cout << "\n[ SIMULATOR CONTROL PANEL (MANUAL MODE) ]\n";
        std::cout << "1. Send a single LIGHT request\n";
        std::cout << "2. Send a single HEAVY request\n";
        std::cout << "3. Send a BATCH of requests (Overload system)\n";
        std::cout << "4. Process 1 Time Tick (Trigger worker & AutoScaler)\n";
        std::cout << "5. Process 5 Time Ticks (Drain system)\n";
        std::cout << "6. Print System Dashboard\n";
        std::cout << "7. Exit\n";
        std::cout << "Enter choice: ";

        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 7)
        {
            std::cout << "Shutting down simulator...\n";
            break;
        }

        int batchSize = 0;
        if (choice == 3)
        {
            std::cout << "Enter batch size (e.g., 15): ";
            std::cin >> batchSize;
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
        {
            std::cout << "[*] Processing 1 system tick...\n";
            lb.processTick();
            break;
        }
        case 5:
        {
            std::cout << "[*] Processing 5 system ticks...\n";
            for (int i = 0; i < 5; i++)
            {
                lb.processTick();
            }
            break;
        }
        case 6:
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