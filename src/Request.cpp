#include "../include/Request.hpp"
Request::Request(int id)
{
    this->id = id;
    timestamp = time(nullptr);
}
int Request::getId()
{
    return id;
}
time_t Request::getTimestamp()
{
    return timestamp;
}