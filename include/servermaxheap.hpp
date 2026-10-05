#ifndef SERVERMAXHEAP_HPP//include guard to prevent multiple inclusions of the header file
#define SERVERMAXHEAP_HPP//define the include guard
#include <vector>//include the vector header for using std::vector
#include "server.hpp"//include the server header for using the Server class
class ServerMaxHeap{
    private:
        std::vector<Server*> heap;
        //define priority logic
        bool isHigherPriority(Server *a,Server *b){
            if(a->getCurrentLoad()>b->getCurrentLoad()){
                return true;
            }
            else if(a->getCurrentLoad()==b->getCurrentLoad()){//tie-breaker based on server id
                return a->getId() < b->getId();
            }
            else{
                return false;
            }
        }
        void siftup(){}
};

#endif//end of the include guard