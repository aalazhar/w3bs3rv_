#pragma once

#include <iostream>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include "RequestClass.hpp"


#define BACKLOG 10

class creatSocket{
    private:
        int sockFd;
        int binding;
        int listning;
        int nwSock;;
        sockaddr_in Addr;
        creatSocket();
    public:
        creatSocket(int domain, int type, int protocol, int port, unsigned int interface);

        void testConnection(const int&, const std::string&);
        void BindAndListenSocket();

        const sockaddr_in &getSockAddr();
        const  int &getSockFd();
        const  int &getlistning();
        const int &getBinding();

        void launch();
};