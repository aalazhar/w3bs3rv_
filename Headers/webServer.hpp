#pragma once

#include "Headers.hpp"
#include "Server.hpp"
#include "Response.hpp"
#include "RequestClass.hpp"

#define TIMEOUT 10


typedef std::vector<Server> ServerVec;

class webServ{
    private:
        // ServerMap _mySrvs;
        ServerVec Servers;;
    public:
        webServ(const std::string&);


        ServerVec::iterator				getItbegin();
        ServerVec::iterator				getItend();
        ServerVec						&getServers();
    	ServerVec::iterator				getServClien(int fd);
        
        
        void 							creatServers(parserObject &);
        void 							lunche();
        std::string 					storeClientIP(int clientSocket) ;
        int 							acceptNewCl(int, int&, ServerVec::iterator &);
        int								readData(int& ,int& , struct kevent &);
        int								sendData(int& ,int& , struct kevent &);
        void							testConnection(const int&, const std::string&);
        void							keventUP(int kq, int fd, int filter, int flag);
        void							Timeout();

};
