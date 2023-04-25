#pragma once

#include "Headers.hpp"


class Req;

class Server{
    private:
        int port;
        
        int sockFd;
        sockaddr_in Addr;

        std::map<int, Req> ClientMap;
        struct config &ServerConfig;
        
    public:
        Server(struct config &);
        int creatSocket();
        int getport(struct config &);
        void testConnection(const int&, const std::string&);
        std::map<int, Req> &getClientMap(){return this->ClientMap;}
        

};