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
    return max_capacity;
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

bool Server::assignRequest(const Request& req) {
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
    requestNode* current = queue_head;
    while (current != nullptr) {
        if (current->data.remaining_ticks > 0) {
            current->data.remaining_ticks--;
        }
        current = current->next;
    }
}

bool Server::popCompletedRequest(Request& out_completed_req) {
    requestNode* prev = nullptr;
    requestNode* current = queue_head;

    while (current != nullptr) {
        if (current->data.remaining_ticks <= 0) {
            out_completed_req = current->data;
            if (prev == nullptr) {
                queue_head = current->next;  
            } else {
                prev->next = current->next;
            }

            if (current == queue_tail) {
                queue_tail = prev;    
            }

            delete current;
            current_load--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;        
}