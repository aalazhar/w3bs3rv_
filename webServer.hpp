#pragma once

#include "Headers.hpp"
#include "Server.hpp"

class parserObject;
// class Server;

class webServ{
    private:
        std::map<int, Server> _mySrvs;

    public:
        webServ(const std::string&);
        std::map<int, Server>::iterator getItbegin(){return this->_mySrvs.begin();}

        std::map<int, Server>::iterator getItend(){return this->_mySrvs.end();}
        std::map<int, Server> &getServers(){return this->_mySrvs;}
        /*
        1 - initial the data from the config file
        1.5 - check the servers
        2 - create sockets for any server and bindin it , liste....
        3 - create the kqueue and add all servers in the kqueue
        4 - handle the while(true) {}
        
        */
       void creatServers(parserObject &);
       void lunche();
       std::map<int, Server>::iterator getServClien(int fd){
            for(std::map<int, Server>::iterator it = this->_mySrvs.begin();it != this->_mySrvs.end(); it++){
                if (it->second.getClientMap().find(fd) != it->second.getClientMap().end())
                    return it;
            }
            return this->getItend();
       }

        void testConnection(const int&, const std::string&);
};

