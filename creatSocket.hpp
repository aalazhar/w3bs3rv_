#pragma once

#include <iostream>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <sys/event.h>
#include <vector>
#include <map>
#include "RequestClass.hpp"


#define BACKLOG 10
#define MAXEVENT 16

class creatSocket{
    private:
        int sockFd;
        int binding;
        int listning;
        int nwSock;;
        int kq;
        struct kevent evSet;
        sockaddr_in Addr;

        std::vector<int> clients;
        creatSocket();
    public:
        creatSocket(int domain, int type, int protocol, int port, unsigned int interface);

        void testConnection(const int&, const std::string&);
        void BindAndListenSocket();

        const sockaddr_in &getSockAddr();
        const  int &getSockFd();
        const  int &getlistning();
        const int &getBinding();
        std::vector<Req> checkComReq(std::map<int, Req>);
        int checkReq(const std::string&);
        void launch();
};

std::ostream &operator<<(std::ostream &, Req &);