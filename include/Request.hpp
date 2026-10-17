#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <ctime>

class Request
{
    private:
        int id;
        time_t timestamp;
    public:
        Request(int id);
        int getId();
        time_t getTimestamp();
};
#endif