#pragma once

#include "Headers.hpp"

#include "parserObjectU.hpp"
#include "RESP.hpp"

// class Response;

class Server{
    private:
        int port;
        
        int sockFd;
        sockaddr_in Addr;
        struct config &ServerConfig;
        
    public:
        Server(struct config &);
        int creatSocket();
        void testConnection(const int&, const std::string&);

        struct config &getConfig();
        int getport() {return this->port;}
        int getSock();
        int getServerFd(int);

};