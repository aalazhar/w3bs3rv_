#pragma once

#include "Headers.hpp"
// #include "parserObjectU.hpp"
// #include "RequestClass.hpp"

#include "Server.hpp"

// class parserObject;
// class Server;

typedef std::map<int, Server> ServerMap;

class webServ{
    private:
        std::map<int, Server> _mySrvs;

    public:
        webServ(const std::string&);
        ServerMap::iterator getItbegin(){return this->_mySrvs.begin();}

        ServerMap::iterator getItend(){return this->_mySrvs.end();}
        ServerMap &getServers(){return this->_mySrvs;}
        /*
        1 - initial the data from the config file
        1.5 - check the servers
        2 - create sockets for any server and bindin it , liste....
        3 - create the kqueue and add all servers in the kqueue
        4 - handle the while(true) {}
        
        */
       void creatServers(parserObject &);
       void lunche();
       ServerMap::iterator getServClien(int fd){
            for(ServerMap::iterator it = this->_mySrvs.begin();it != this->_mySrvs.end(); it++){
                // it->second.printIt();
                if (it->second.getClientMap().find(fd) != it->second.getClientMap().end())
                    return  it;
            }
            return this->getItend();
       }

        void testConnection(const int&, const std::string&);
};

