#include"clusterManager.hpp"

ClusterManager::ClusterManager():
    head(nullptr),
    tail(nullptr),
    total_servers(0),
    next_server_id(1) {}

ClusterManager::~ClusterManager(){
    clearAll();
}

void ClusterManager::clearAll(){
    ServerNode* current = head;
    while(current != nullptr){
        ServerNode* next_node = current->next;
        delete current->server;
        delete current;
        current = next_node;
    }
    head = nullptr;
    tail = nullptr;
    total_servers = 0;
}

int ClusterManager::getTotalServers(){
    return total_servers;
}

bool ClusterManager::isAtMaxCapacity(){
    return total_servers >= SystemConfig::MAX_CLUSTER_SIZE;
}

Server* ClusterManager::addServer(){
    if(isAtMaxCapacity()){
        return nullptr;
    }
    Server* new_server = new Server(next_server_id++);
    ServerNode* new_node = new ServerNode(new_server);

    if(head == nullptr){
        head = tail = new_node;
    }
    else{
        tail->next = new_node;
        new_node->prev = tail;
        tail = new_node;
    }
    total_servers++;
    return new_server;
}

bool ClusterManager::removeIdleServer(){
    ServerNode* current = head;
    while(current != nullptr){
        //only ACTIVE idle server, crashed or INACTIVE must stay in list for reporting
        if(current->server->getStatus() == ServerStatus::ACTIVE && current->server->getCurrentLoad() == 0){
            if(current->prev != nullptr){
                current->prev->next = current->next;
            }
            else{
                head = current->next;
            }

            if(current->next != nullptr){
                current->next->prev = current->prev;
            }
            else{
                tail = current->prev;
            }
            delete current->server;
            delete current;
            total_servers--;
            return true;
        }
        current = current->next;
    }
    return false;
}

