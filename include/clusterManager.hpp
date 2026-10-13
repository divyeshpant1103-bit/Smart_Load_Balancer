#ifndef CLUSTER_MANAGER_H
#define CLUSTER_MANAGER_H

#include"server.hpp"
#include<vector>

struct ServerNode{
    Server* server;
    ServerNode* prev;
    ServerNode* next;

    ServerNode(Server* srv): server(srv), prev(nullptr), next(nullptr) {}
};

class ClusterManager{
    private:
        ServerNode* head;
        ServerNode* tail;
        int total_servers;
        int next_server_id;

    public:
        ClusterManager();
        ~ClusterManager();

        ClusterManager(const ClusterManager&) = delete;
        ClusterManager& operator = (const ClusterManager&) = delete;
        
        Server* addServer();
        bool removeIdleServer();
        Server* findServer(int id);
        std::vector<Server*> getActiveServers();

        int getTotalServers();
        bool isAtMaxCapacity();
        void clearAll();
};

#endif
