#pragma once

#include "Headers.hpp"
#include "Server.hpp"
// #include "Response.hpp"
#include "RequestClass.hpp"
#include "RESP.hpp"




typedef std::vector<Server> ServerVec;
typedef std::map<int, Res> _ClientMap;

class webServ{
    private:
        parserObject *ob;
        ServerVec Servers;;
        _ClientMap Cmap;
    public:
        webServ(const std::string&);
        ~webServ();


        // ServerVec::iterator				getItbegin();
        // ServerVec::iterator				getItend();
        ServerVec						&getServers();
    	// ServerVec::iterator				getServClien(int fd);
        
        
        void 							creatServers(parserObject*);
        void 							lunche();
        std::string 					storeClientIP(int clientSocket) ;
        int 							acceptNewCl(int, int);
        int								readData(int& ,int& , struct kevent &);
        int								sendData(int ,int);
        static void						ErrorHandler(const int, const std::string);
        static void						keventUP(int kq, int fd, int filter, int flag);
        void							Timeout(int);
        bool                            ifServer(int);
        bool                            checkport(int);
        void                            eraseClient(int,int);

        // ServerVec::iterator getServer(int fd){
        //     std::vector<Server>::iterator it = this->Servers.begin();
        //     for(;it != this->Servers.end(); it++){
        //         if (it->getSock() == fd)
        //             return it;
        //     }
        //     return it;
        // }

};
