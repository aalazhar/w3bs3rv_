#include "../Headers/webServer.hpp"
#define MAXEVENT 64

webServ::webServ(const std::string &filename){
    parserObject obj(filename);
    try{
        obj.open_config_file();

    }catch(...){
        exit(1);
    }
    this->Servers.clear();
    creatServers(obj);
}

void webServ::creatServers(parserObject &obj){
    std::vector<config>::iterator it = obj.getItBegin();
    for (;it != obj.getItend(); it++){
        Server s(*it);
		s.creatSocket();
        this->Servers.push_back(s);
    }
}

void ft_err(){
    exit(1);
}

void webServ::keventUP(int kq, int fd, int filter, int flag){
    struct kevent ev;
    // timespec timeout = 
    EV_SET(&ev, fd, filter, flag, 0, 0, NULL);
    testConnection (kevent(kq, &ev, 1, NULL, 0, NULL), "FAILDE ADD THE FD TO KEVENT");
}


std::string webServ::storeClientIP(int clientSocket) {
    struct sockaddr_storage addr;
    socklen_t addrLength = sizeof(addr);

    // Get the client's address information
    if (getpeername(clientSocket, (struct sockaddr*)&addr, &addrLength) == 0) {
        if (addr.ss_family == AF_INET) {
            // IPv4 address
            struct sockaddr_in* s = (struct sockaddr_in*)&addr;
            char ipAddress[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(s->sin_addr), ipAddress, INET_ADDRSTRLEN);
            return std::string(ipAddress);
        } else if (addr.ss_family == AF_INET6) {
            // IPv6 address
            struct sockaddr_in6* s = (struct sockaddr_in6*)&addr;
            char ipAddress[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &(s->sin6_addr), ipAddress, INET6_ADDRSTRLEN);
            return std::string(ipAddress);
        } else {
            return "Unknown address family";
        }
    } else {
        perror("getpeername");
        return "Error retrieving client IP";
    }
}

void webServ::lunche(){
    int kq = kqueue();
    for (ServerVec::iterator it = this->Servers.begin(); it != this->Servers.end(); it++)
        keventUP(kq, it->getSock(), EVFILT_READ, EV_ADD);
    
    // int clientSock = 0;
    while (true){
        struct timespec timeout;
        timeout.tv_sec = TIMEOUT;
        timeout.tv_nsec = 0;
		struct kevent events[MAXEVENT];
		int n_event = kevent(kq, NULL, 0, events, MAXEVENT, &timeout);
        for (int i = 0; i < n_event; i++)
        {
            int fd = events[i].ident;
            // ServerVec::iterator itS = this->getServClien(fd);
            if (events[i].flags & EV_EOF)
            {
                std::cout << "----EOF-----\n";
                keventUP(kq, fd,  EVFILT_READ , EV_CLEAR | EV_DELETE);

                std::cout <<"client "<< fd << " is disconnected\n";
                // itS->second.eraseClient(fd);
                    Cmap.erase(fd);
                    close(fd);

                std::cout << "----END_EOF-----\n";
            }
            else if ( this->ifServer(fd) )
            { //new client
                // std::cout << "fd is serfer\n";
				// itS = this->getServer(fd);
                // int ServerFD = 
                if (acceptNewCl(kq, fd) < 0)
                    continue;
            }
            else if (events[i].filter == EVFILT_READ) // else if (read data)
            {
                if (readData(kq ,fd, events[i]) < 0)
                    continue;;
            } 
            else if (events[i].filter == EVFILT_WRITE)
            {
                if (sendData(kq, fd, events[i]) < 0)
                    continue;
            }
        }
        // this->Timeout();
    }
}

// int Server::getServerFd(int fd) {
    
//     _ClientMap pp;

//     std::map<int, Response> it;
//     if (it = pp.find(fd) != pp.end())

//     it->first;
// }


// void webServ::Timeout(){

// }



void webServ::testConnection(const int& test, const std::string& msg){
    if (test < 0){
        std::cerr << "Faild to connection ?!: " << msg << std::endl;
		std::cerr << "Error : " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
    }
}


int webServ::acceptNewCl(int kq, int ServerSock){
    std::cout << "\n-------accepteNewCl-------\n";
    int clientSock;
    struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
    clientSock = accept(ServerSock, (struct sockaddr*)&client_addr, &client_addr_len);
    size_t i = 0;
    while(i < Servers.size()){
        if (Servers[i].getSock() == ServerSock)
            break;
    }
    std::cout << " CLientSock = " << clientSock << "\n";
    Response rs(Servers[i].getConfig(), ServerSock, clientSock);
    std::cout << "waa zzeeeb\n";
    testConnection(clientSock, "accepte a new client");
    // try{
        this->Cmap.insert(std::make_pair(clientSock, rs));
    // }catch(...){
    //     std::cout << "errrrrror\n";
    // }
    keventUP(kq, clientSock, EVFILT_READ, EV_ADD);
    std::cout << "----Accepted new client connection on socket----\n";
    return 0;
}

int webServ::sendData(int &kq,int& fd, struct kevent &event){
    std::cout << "\n------send data---------\n";

    // ServerVec::iterator Server = getServClien(fd);
    int ServerFd = Cmap.find(fd)->second.getServerFd();
    size_t i = 0;
    for(; i < Servers.size(); i++){
        if (Servers[i].getSock() == ServerFd)
            break;;
    }
    std::cout << "server n = " << Servers[i].getSock() << "map size = " << this->Cmap.size() << "\n";
    Response &res = this->Cmap.find(fd)->second;
    std::cout << "-----request : ------\n" << *dynamic_cast<Req*>(&res) << "\n------------\n";
    if (res.getR() == 0)
        res.makeResponse();
    std::cout << "------------\n";
    std::string response = res.getStatusLine() + CRLF + res.getheaders() + CRLF + res.getResponse_body();
    std::cout << "---RESPONS---\n| " << response << "|\n------------\n";
    std::cout << "r = " << res.getR() << std::endl;
    int length = event.data;
    const char *buff;
    if (res.getR() < response.length())
        buff = &response.c_str()[res.getR()];
        std::cout << "BUFF : " << buff <<std::endl;
    if (send(fd, response.c_str(), length, 0) < 0)
        std::cout << "send() < 0\n";
    res.setR(res.getR() + length);
    if (res.getR() > response.length())
    {
        keventUP(kq, fd, EVFILT_WRITE, EV_DISABLE);
        keventUP(kq, fd, EVFILT_READ, EV_CLEAR | EV_ENABLE | EV_ADD);
        res.clear();
        // Server->eraseClient(fd);
    }
    else{
        keventUP(kq, fd, EVFILT_WRITE, EV_ENABLE);
    }
    std::cout << "-------f snd data--------\n";
    return 0;

}

int webServ::readData(int &kq, int& fd, struct kevent &event){
    std::cout << "\n------read data---------\n";
    // ServerVec::iterator Server = getServClien(fd);
    int ServerFd = Cmap.find(fd)->second.getServerFd();
    size_t i = 0;
    for(; i < Servers.size(); i++){
        if (Servers[i].getSock() == ServerFd)
            break;;
    }
    // Req *r;
    std::cout << "server n = " << Servers[i].getSock() << "map size = " << this->Cmap.size() << "\n" ;
    char buffer[event.data];
    // _ClientMap m;
    std::cout << "fd = " << fd  << "   size : " << event.data << std::endl;
    memset(buffer, 0, event.data);
    int rd = recv(fd, buffer, event.data, 0);
    if (rd <= 0)
    {
        if (rd == 0)
            std::cout <<"client "<< fd << " is disconnected\n";
		else
			std::cout << "Error receving data from client\n";
		//erase client  from _map
        Cmap.erase(fd);
        // Server->eraseClient(fd);
        return (-1);
    }
    else
    {
        buffer[event.data] = 0;
        std::string req(buffer); 
        //append the read string in the request class
        Cmap.find(fd)->second.append(req);
        // r.append(req);
        std::cout << *dynamic_cast<Req *>(&Cmap.find(fd)->second) << std::endl;;
    }
        std::cout << "r = " << Cmap.find(fd)->second.getStep() << std::endl;;
    if (Cmap.find(fd)->second.getStep() == DONE)
    {
		keventUP(kq, fd, EVFILT_READ, EV_DISABLE);
        keventUP(kq, fd, EVFILT_WRITE, EV_CLEAR|EV_ENABLE | EV_ADD);
    }
    std::cout << "--------finish read data---------\n";
    return 0;
}


// ServerVec::iterator webServ::getItbegin(){
//     return this->Servers.begin();
// }

// ServerVec::iterator webServ::getItend(){
//     return this->Servers.end();
// }

std::vector<Server> &webServ::getServers(){
    return this->Servers;
}

// int webServ::getServClien(int fd){
//     // for(ServerVec::iterator it = this->Servers.begin(); it != this->Servers.end(); it++){

//     // }
//     ServerVec::iterator it = this->Servers.begin();
//     for(;it != this->Servers.end(); it++){
//         std::cout << "server fd = " << it->getSock() << std::endl;
//         std::cout << "fd = " << fd << "    server size -> " << this->Servers.size()  << std::endl;
//         if (it->getClientMap().find(fd) != it->getClientMap().end())
//             return  (std::cout << "gat client Succes\n",it);
//     }
//     std::cout << "get Client Faild\n";
//     return it;
// }

