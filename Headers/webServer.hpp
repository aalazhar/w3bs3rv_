#pragma once

#include "Headers.hpp"
#include "Server.hpp"
#include "RequestClass.hpp"
#include "ResponseClass.hpp"

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

        void 							creatServers(parserObject*);
        void 							lunche();
        int 							acceptNewCl(int, int);
        int								readData(int& ,int& , struct kevent &);
        int								sendData(int ,int);
        static void						keventUP(int kq, int fd, int filter, int flag);
        static void						ErrorHandler(const int, const std::string);
        void							Timeout(int);
        bool                            ifServer(int);
        bool                            checkport(int);
        void                            eraseClient(int,int);

        std::string 					storeClientIP(int clientSocket) ;
        ServerVec						&getServers();
        size_t                          getServerIndex(int);
};
