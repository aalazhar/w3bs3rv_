#pragma once

#include "Headers.hpp"
#include "creatSocket.hpp"
#include "RequestClass.hpp"
#include "Server.hpp"


class webServ{
    private:
        std::map<int, Server> _mySrvs;

    public:
        webServ(const std::string&);
        /*
        1 - initial the data from the config file
        1.5 - check the servers
        2 - create sockets for any server and bindin it , liste....
        3 - create the kqueue and add all servers in the kqueue
        4 - handle the while(true) {}
        
        */
       
       void lunche();
};

