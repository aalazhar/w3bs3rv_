#pragma once

#include "Headers.hpp"
#include "Server.hpp"
#include "Response.hpp"
#include "RequestClass.hpp"

#define TIMEOUT 10


typedef std::vector<Server> ServerVec;
typedef std::map<int, Response> _ClientMap;

class webServ{
    private:
        // ServerMap _mySrvs;
        ServerVec Servers;;
        _ClientMap Cmap;
    public:
        webServ(const std::string&);


        // ServerVec::iterator				getItbegin();
        // ServerVec::iterator				getItend();
        ServerVec						&getServers();
    	// ServerVec::iterator				getServClien(int fd);
        
        
        void 							creatServers(parserObject &);
        void 							lunche();
        std::string 					storeClientIP(int clientSocket) ;
        int 							acceptNewCl(int, int);
        int								readData(int& ,int& , struct kevent &);
        int								sendData(int& ,int& , struct kevent &);
        void							testConnection(const int&, const std::string&);
        void							keventUP(int kq, int fd, int filter, int flag);
        // void							Timeout();
        bool ifServer(int fd){
            for(ServerVec::iterator it = this->Servers.begin();it != this->Servers.end(); it++){
                if (it->getSock() == fd)
                    return true;
            }
            return false;
        }

        // ServerVec::iterator getServer(int fd){
        //     std::vector<Server>::iterator it = this->Servers.begin();
        //     for(;it != this->Servers.end(); it++){
        //         if (it->getSock() == fd)
        //             return it;
        //     }
        //     return it;
        // }

};
