#ifndef SERVER_H
#define SERVER_H

#include "common.hpp"

//custom node for the Server's internal request queue assigned to it currently
struct requestNode{
    Request data;
    requestNode* next;

    requestNode(const Request& req): data(req), next(nullptr) {}
};

class Server{
    private:
        int server_id;
        int max_capacity;
        ServerStatus status;
        requestNode* queue_head;
        requestNode* queue_tail;
        int current_load;

    public:
        Server(int id, int capacity = SystemConfig::MAX_SERVER_CAPACITY);
        ~Server();

        //Prevent copying
        Server(const Server&) = delete;
        Server& operator = (const Server&) = delete;
        
        int getId();
        int getMaxCapacity();
        int getCurrentLoad();
        double getUtilizationPercentage();
        ServerStatus getStatus();
        void setStatus(ServerStatus new_status);

        bool assisgnRequest(const Request& req);
        void processTick();
        bool popCompletedRequest(Request& completed_req);
        void clearQueue();
        requestNode* getHead() { return queue_head; }

};

#endif