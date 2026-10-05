#include"server.hpp"

Server::Server(int id, int capacity) :
    server_id(id),
    max_capacity(capacity),
    status(ServerStatus::ACTIVE),
    queue_head(nullptr),
    queue_tail(nullptr),
    current_load(0) {}

Server::~Server() {
    clearQueue();
}

void Server::clearQueue() {
    requestNode* current = queue_head;
    while(current != nullptr){
        requestNode* next_node = current->next;
        delete current;
        current = next_node;
    }
    queue_head = nullptr;
    queue_tail = nullptr;
    current_load = 0;
}

int Server::getId() {
    return server_id;
}

int Server::getCurrentLoad() {
    return current_load;
}

int Server::getMaxCapacity() {
    return current_load;
}

double Server::getUtilizationPercentage() {
    if(max_capacity == 0) return  0;
    return (static_cast<double>(current_load) / max_capacity)*100.0;
}

ServerStatus Server::getStatus() {
    return status;
}

void Server::setStatus(ServerStatus new_status){
    status = new_status;
}

bool Server::assisgnRequest(const Request& req) {
    if(status != ServerStatus::ACTIVE || current_load >= max_capacity){
        return false;
    }
    requestNode* new_node = new requestNode(req);
    if(queue_head == nullptr){
        queue_head = queue_tail = new_node;
    }
    else{
        queue_tail->next = new_node;
        queue_tail = new_node;
    }
    current_load++;
    return true;
}

void Server::processTick() {
    if(queue_head == nullptr) return;
    queue_head->data.remaining_ticks --;
}

bool Server::popCompletedRequest(Request& out_completed_req) {
    if(queue_head == nullptr || queue_head->data.remaining_ticks > 0){
        return false;
    }
    requestNode* tmp = queue_head;
    out_completed_req = tmp->data;
    queue_head = queue_head->next;
    if(queue_head == nullptr){
        queue_tail = nullptr;
    }
    delete tmp;
    current_load --;
    return true;
}