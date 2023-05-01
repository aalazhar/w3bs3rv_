#pragma once

#include "Headers.hpp"

#include "RequestClass.hpp"
#include "parserObjectU.hpp"

// class Req;

typedef std::map<int, Req*> _ClientMap;

class Server{
    private:
        int port;
        
        int sockFd;
        sockaddr_in Addr;

        _ClientMap Cmap;
        struct config &ServerConfig;
        
    public:
        Server(struct config &);
        int creatSocket();
        int getport(struct config &);
        void testConnection(const int&, const std::string&);
        _ClientMap &getClientMap(){return  this->Cmap;}
        void printIt(){
            _ClientMap::iterator it = Cmap.begin();
            for(; it != Cmap.end(); it++){
                std::cout << "Server[" << sockFd << "] == client fd = " << it->first <<std::endl;
            }
        }
        _ClientMap::iterator getClientBegin() {return this->Cmap.begin();}
        _ClientMap::iterator getClientEnd() {return this->Cmap.end();}

};