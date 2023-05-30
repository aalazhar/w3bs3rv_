#include "../Headers/webServer.hpp"
#define MAXEVENT 64

webServ::webServ(const std::string &filename){
    this->ob = new parserObject(filename);
    try{
        ob->open_config_file();

    }catch(...){
        exit(1);
    }
    this->Servers.clear();
    creatServers(this->ob);
}

webServ::~webServ(){
    delete this->ob;
}

void webServ::ErrorHandler(const int test, const std::string msg){
    if (test < 0){
        std::cerr << msg << std::endl;
		std::cerr << "Error : " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
    }
}

bool webServ::ifServer(int fd){
    for(ServerVec::iterator it = this->Servers.begin();it != this->Servers.end(); it++){
        if (it->getSock() == fd)
            return true;
    }
    return false;
}

bool webServ::checkport(int port){
    for (size_t i = 0; i < Servers.size(); i++){
        if (Servers[i].getport() == port)
            return true;
    }
    return false;
}

void webServ::creatServers(parserObject *obj){
    std::vector<config>::iterator it = obj->getItBegin();
    for (;it != obj->getItend(); it++){
        Server s(*it);
        if (checkport(s.getport()) == true)
            ErrorHandler(-1337, "Duplicate Port in Config File");
		s.creatSocket();
        this->Servers.push_back(s);
    }
    std::cout << "hellllllll\n";
}

void webServ::keventUP(int kq, int fd, int filter, int flag){
    struct kevent ev;
    EV_SET(&ev, fd, filter, flag, 0, 0, NULL);
    if (kevent(kq, &ev, 1, NULL, 0, NULL) < 0)
        std::cout << "FAILDE ADD THE FD TO KEVENT" << std::endl;
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
	struct kevent events[MAXEVENT];
    struct timespec timeout;
    timeout.tv_sec = 10;  
    timeout.tv_nsec = 0;
    int n_event = 0;
    int fd = 0;
    
    for (ServerVec::iterator it = this->Servers.begin(); it != this->Servers.end(); it++)
        keventUP(kq, it->getSock(), EVFILT_READ, EV_ADD);

    for (;;){

		n_event = kevent(kq, NULL, 0, events, MAXEVENT, &timeout);
        if (n_event == 0)
            this->Timeout(kq);
        else if (n_event > 0){
            for (int i = 0; i < n_event; i++)
            {
                fd = events[i].ident;
                if (events[i].flags & EV_EOF)
                    eraseClient(kq, fd);
                else if ( this->ifServer(fd) ) //ACCEPT NEW CONNECTION
                    acceptNewCl(kq, fd);
                else if (events[i].filter == EVFILT_READ) // READ DATA
                    readData(kq ,fd, events[i]);
                else if (events[i].filter == EVFILT_WRITE) //SEND DATA
                    sendData(kq, fd);
            }
        }
    }
}


void webServ::eraseClient(int kq,int fd){

    std::cout << "----EOF-----\n";
    keventUP(kq, fd, EVFILT_WRITE, EV_CLEAR | EV_DELETE);
    keventUP(kq, fd, EVFILT_READ, EV_CLEAR | EV_DELETE);
    std::cout <<"client "<< fd << " is disconnected\n";
    Cmap.erase(fd);
    close(fd);
    std::cout << "----END_EOF-----\n";
}

void webServ::Timeout(int kq){

    for (_ClientMap::iterator it = this->Cmap.begin(); it != this->Cmap.end(); it++){
        std::cout << "TIME OUT" << std::endl;
        if (std::time(NULL) - it->second.getTime() > 10)
        {
            std::cout << "TIME OUT IF" << std::endl;
            it->second.setStep(-2);
        	keventUP(kq, it->first, EVFILT_READ, EV_DISABLE);
            keventUP(kq, it->first, EVFILT_WRITE, EV_CLEAR|EV_ENABLE | EV_ADD);
        }
    }
    std::cout << "TIME OUT KHREJ" << std::endl;
}


int webServ::acceptNewCl(int kq, int ServerSock){
    std::cout << "\n-------accepteNewCl-------\n";
    int clientSock;
    struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
    clientSock = accept(ServerSock, (struct sockaddr*)&client_addr, &client_addr_len);
    ErrorHandler(clientSock, "accepte a new client");

    ErrorHandler(fcntl(clientSock, F_SETFL, O_NONBLOCK), "fnctl");
    size_t i = 0;
        std::cout << "serversize = " << Servers.size() << std::endl;
    while(i < Servers.size()){
        if (Servers[i].getSock() == ServerSock)
            break;
        i++;
    }
    std::cout << " CLientSock = " << clientSock << "\n";
    Res rs(Servers[i].getConfig(), ServerSock, clientSock);
    std::cout << "waa zzeeeb\n";
    this->Cmap.insert(std::make_pair(clientSock, rs));
    keventUP(kq, clientSock, EVFILT_READ, EV_ADD);
    std::cout << "----Accepted new client connection on socket----\n";
    return 0;
}

/*_______SEND DATA___________*/
/*_______SEND DATA___________*/
/*_______SEND DATA___________*/

int webServ::sendData(int kq,int fd){
    std::cout << "\n------send data--------"<< std::endl;
    int ServerFd = Cmap.find(fd)->second.getServerFd();
    size_t i = 0;
    for(; i < Servers.size(); i++){
        if (Servers[i].getSock() == ServerFd)
            break;;
    }
    std::cout << "server n = " << Servers[i].getSock() << "map size = " << this->Cmap.size() << std::endl;;
    Res &res = this->Cmap.find(fd)->second;
    res.buildResponse();
    size_t length = res.getResp().size();
    if (send(fd, res.getResp().data(), length, 0) < 0)
        std::cout << "SEND FAILD\n";
    keventUP(kq, fd, EVFILT_WRITE, EV_DISABLE);
	keventUP(kq, fd, EVFILT_READ, EV_CLEAR | EV_ENABLE | EV_ADD);
    std::cout << "SSSTTTEEEP = " << res.getStep() << "     if " << (res.getStep() == TIMEOUT) << std::endl;;
    if (res.getStep() == TIMEOUT)
        eraseClient(kq, fd);
    else
	    res.resetvalues();
    std::cout << "LLLLLLL"  << "------------" << std::endl;;
    return 0;

}


/*__________READ DATA___________*/
/*__________READ DATA___________*/
/*__________READ DATA___________*/


std::vector<char> addtoVec(const char *s, size_t length){
    std::vector<char> vec;
    for (size_t i = 0; i < length; i++)
        vec.push_back(s[i]);
        return vec;
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
    // std::cout << "server n = " << Servers[i].getSock() << "map size = " << this->Cmap.size() << "\n" ;
    char buffer[event.data];
    std::vector<char> vec;
    vec.clear();
    std::cout << "fd = " << fd  << "   size : " << event.data << std::endl;
    int rd = recv(fd, buffer, event.data, 0);
    if (rd <= 0)
        return (eraseClient(kq, fd),-1);
    vec = addtoVec(buffer, event.data);
    std::string req(vec.begin(), vec.end());

    //append the read string in the request class
    Cmap.find(fd)->second.append(req);
    std::cout << *dynamic_cast<Req *>(&Cmap.find(fd)->second) << std::endl;;
    std::cout << "khdem a zbi " << std::endl;;
    if (Cmap.find(fd)->second.getStep() > 2 || Cmap.find(fd)->second.getStep() < 0)
    {
        std::cout << "daz mn hna" << std::endl;;
		keventUP(kq, fd, EVFILT_READ, EV_DISABLE);
        keventUP(kq, fd, EVFILT_WRITE, EV_CLEAR|EV_ENABLE | EV_ADD);
    }
    std::cout << "--------finish read data 1---------" << std::endl;
    Cmap.find(fd)->second.updateTime();
    std::cout << "--------finish read data---------" << std::endl;
    return 0;
}

std::vector<Server> &webServ::getServers(){
    return this->Servers;
}

