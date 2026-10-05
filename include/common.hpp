#ifndef COMMON_H
#define COMMON_H

#include<iostream>
#include<string>

enum class RequestType{
    LIGHT = 1,
    MEDIUM = 3,
    HEAVY = 5
};

enum class ServerStatus{
    ACTIVE,
    DRAINING,
    INACTIVE
};

struct Request{
    int request_id;
    RequestType type;
    int remaining_ticks;
    int arrival_time;

    Request(int id, RequestType req_type, int arrival){
        request_id = id;
        type = req_type;
        arrival_time = arrival;
        switch(type){
            case RequestType::LIGHT:
                remaining_ticks = 1;
                break;
            case RequestType::MEDIUM:
                remaining_ticks = 3;
                break;
            case RequestType::HEAVY:
                remaining_ticks = 5;
                break;
        }
    }

    std::string getType() const{
        switch(type){
            case RequestType::LIGHT : return "LIGHT";
            case RequestType::MEDIUM : return "MEDIUM";
            case RequestType::HEAVY : return "HEAVY";
            default : return "UNKNOWN";
        }
    }

    struct SystemConfig{
        static constexpr int INITIAL_SERVER_COUNT = 3;
        static constexpr int MAX_SERVER_CAPACITY = 10;
        static constexpr int SCALE_UP_THRESHOLD = 75;
        static constexpr int SCALE_DOWN_THRESHOLD = 25;
        static constexpr int SCALING_STREAK_LIMIT = 2;
    }
};

#endif