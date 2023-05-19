#include "../Headers/webServer.hpp"
#define MAXEVENT 64
void ft_err(){
    exit(1);
}

void webServ::keventUP(int kq, int fd, int filter, int flag){
    struct kevent ev;
    // timespec timeout = 
    EV_SET(&ev, fd, filter, flag, 0, 0, NULL);
    testConnection (kevent(kq, &ev, 1, NULL, 0, NULL), "FAILDE ADD THE FD TO KEVENT");
}

// int checkConfig(config &c){
//     (void)c;
//     return 0;
// }

// void checkObj(parserObject &obj){
//     std::vector<config>::iterator it = obj.getItBegin();
//     while (it != obj.getItend()){
//         if (checkConfig(*it) < 0)
//             ft_err();
//         it++;
//     }

// }



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

void webServ::creatServers(parserObject &obj){
    std::vector<config>::iterator it = obj.getItBegin();
    for (;it != obj.getItend(); it++){
        Server s(*it);
        std::cout << "zbiiiiii " <<  s.creatSocket() << "\n";
        this->Servers.push_back(s);
        // this->_mySrvs.insert(std::make_pair(s.creatSocket(),s));
    }
}


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




void webServ::lunche(){
    int kq = kqueue();
    for (ServerVec::iterator it = getItbegin(); it != getItend(); it++)
        keventUP(kq, it->getSock(), EVFILT_READ, EV_ADD);
    
    int clientSock = 0;
    while (true){
        struct timespec timeout;
        timeout.tv_sec = TIMEOUT;
        timeout.tv_nsec = 0;
		struct kevent events[MAXEVENT];
		int n_event = kevent(kq, NULL, 0, events, MAXEVENT, &timeout);
        for (int i = 0; i < n_event; i++)
        {
            int fd = events[i].ident;
            ServerVec::iterator itS = this->getServClien(fd);
            if (events[i].flags & EV_EOF)
            {
                std::cout << "----EOF-----\n";
                keventUP(kq, fd,  EVFILT_READ , EV_CLEAR | EV_DELETE);

                std::cout <<"client "<< fd << " is disconnected\n";
                // itS->second.eraseClient(fd);

                std::cout << "----END_EOF-----\n";
            }
            else if ( itS != this->getItend() )
            { //new client 
                if (acceptNewCl(kq, clientSock, itS) < 0)
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
        this->Timeout();
    }

}

void webServ::Timeout(){

}



void webServ::testConnection(const int& test, const std::string& msg){
    if (test < 0){
        std::cerr << "Faild to connection ?!: " << msg << std::endl;
		std::cerr << "Error : " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
    }
}


int webServ::acceptNewCl(int kq, int& clientSock, ServerVec::iterator &Server){
    std::cout << "\n-------accepteNewCl-------\n";
    struct sockaddr_in client_addr;
    std::cout << "server n = " << Server->getSock() << "map size = " << Server->getClientMap().size() << "\n";
	socklen_t client_addr_len = sizeof(client_addr);
    clientSock = accept(Server->getSock(), (struct sockaddr*)&client_addr, &client_addr_len);
    testConnection(clientSock, "accepte a new client");
    std::cout << "fd = " << clientSock << "  IP = " << this->storeClientIP(clientSock)<< "\n";
    try{
        Server->addNewClient(clientSock);
    }catch(...){
        std::cout << "errrrrror\n";
    }
    std::cout << "server n = " << Server->getSock() << " map size = " << Server->getClientMap().size() << "\n";
    keventUP(kq, clientSock, EVFILT_READ, EV_ADD);

	//add the new client socket to the kqueue
	// struct kevent evSet;
	// EV_SET(&evSet, clientSock, EVFILT_READ, EV_ADD, 0, 0, NULL);
	// if (kevent(kq, &evSet, 1, NULL, 0, NULL) < 0)
    //     return std::cerr << "adding client socket to kqueue\n", close(clientSock), -1;
    // keventUP(kq, clientSock, EVFILT_READ, EV_ADD);
    // Server->getClientMap().insert(std::pair<int, Response *>(clientSock, res));

    std::cout << "----Accepted new client connection on socket----\n";
    return 0;
}

int webServ::sendData(int &kq,int& fd, struct kevent &event){
    std::cout << "\n------send data---------\n";
    std::cout << "fd = " << fd << std::endl;
    std::cout << "allowed byte to write : " << event.data << std::endl;

    ServerVec::iterator Server = getServClien(fd);
    std::cout << "server n = " << Server->getSock() << "map size = " << Server->getClientMap().size() << "\n";
    _ClientMap _clientMap = Server->getClientMap();
    Response &res = _clientMap.find(fd)->second;
    std::cout << "-----request : ------\n" << *dynamic_cast<Req*>(&res) << "\n------------\n";
    if (res.getR() == 0)
        res.makeResponse();
    // char *str[event.data];
    std::cout << *dynamic_cast<Req*>(&res) << "--------------\n";
    std::string response = res.getStatusLine() + CRLF + res.getheaders() + CRLF + res.getResponse_body();
    std::cout << "---RESPONS---\n" << response << "\n------------\n";
    
    int length = event.data;
    const char *buff;
    if (res.getR() < response.length())
        buff = &response.c_str()[res.getR()];
        std::cout << "BUFF : " << buff <<std::endl;
    if (send(fd, response.c_str(), length, 0) < 0)
        std::cout << "send() < 0\n";
    res.setR(res.getR() + length);
    // std::cout << (res.getR() > response.length()) << "    lenght" << length << " R" << res.getR() << "\n";
    if (res.getR() > response.length())
    {
        keventUP(kq, fd, EVFILT_WRITE, EV_DISABLE);
        keventUP(kq, fd, EVFILT_READ, EV_CLEAR | EV_ENABLE | EV_ADD);
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
    ServerVec::iterator Server = getServClien(fd);
    Req r;
    std::cout << "server n = " << Server->getSock() << "map size = " << Server->getClientMap().size() << "\n" ;
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
        std::cout << "aaaaaaa\n";
        Server->eraseClient(fd);
        return (-1);
    }
    else
    {
        buffer[event.data] = 0;
        std::string req(buffer); 
        //append the read string in the request class
        Req r = Server->getClientMap().find(fd)->second;
        r.append(req);
        std::cout << "bababababaababa\n";
        std::cout << r;
        
    }
    if (r.getStep() == DONE)
    {
		keventUP(kq, fd, EVFILT_READ, EV_DISABLE);
        keventUP(kq, fd, EVFILT_WRITE, EV_CLEAR|EV_ENABLE | EV_ADD);
    }
    std::cout << "--------finish read data---------\n";
    return 0;
}


std::vector<Server>::iterator webServ::getItbegin(){
    return this->Servers.begin();
}

std::vector<Server>::iterator webServ::getItend(){
    return this->Servers.end();
}

std::vector<Server> &webServ::getServers(){
    return this->Servers;
}

std::vector<Server>::iterator webServ::getServClien(int fd){
    for(std::vector<Server>::iterator it = this->Servers.begin();it != this->Servers.end(); it++){
        if (it->getClientMap().find(fd) != it->getClientMap().end())
            return  it;
    }
    return this->getItend();
}