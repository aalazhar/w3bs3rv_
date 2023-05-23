#pragma once

#include "Headers.hpp"

#include "parserObjectU.hpp"
#include "Response.hpp"

// class Response;

typedef std::map<int, Response> _ClientMap;
typedef std::vector<Response> _ClientVec;

class Server{
    private:
        int port;
        
        int sockFd;
        sockaddr_in Addr;

        _ClientMap Cmap;
        _ClientVec Clientvec;
        struct config &ServerConfig;
        
    public:
        Server(struct config &);
        int creatSocket();
        void testConnection(const int&, const std::string&);
        // void printIt();
        
        // void eraseClient(int fd);

        // void addNewClient(int fd);

        struct config &getConfig();
        // _ClientMap::iterator getClientBegin();
        // _ClientMap::iterator getClientEnd();
        // Response &getResponse(int fd){
        //     return this->Cmap.find(fd)->second;
        // }
        int getport(struct config &);
        // _ClientMap &getClientMap();
        int getSock();
        int getServerFd(int);

};