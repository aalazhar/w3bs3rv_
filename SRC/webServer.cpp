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

void webServ::creatServers(parserObject *obj){
    std::vector<config>::iterator it = obj->getItBegin();
    for (;it != obj->getItend(); it++){
        Server s(*it);
		s.creatSocket();
        this->Servers.push_back(s);
    }
    std::cout << "hellllllll\n";
}

void ft_err(){
    exit(1);
}

void webServ::keventUP(int kq, int fd, int filter, int flag){
    struct kevent ev;
    struct timespec timeout;
     if (clock_gettime(CLOCK_REALTIME, &timeout) == -1) {
        perror("clock_gettime");
        return ;
    }
    EV_SET(&ev, fd, filter, flag, 0, 0, &timeout);
    testConnection(kevent(kq, &ev, 1, NULL, 0, NULL), "FAILDE ADD THE FD TO KEVENT");
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

		struct kevent events[MAXEVENT];
		int n_event = kevent(kq, NULL, 0, events, MAXEVENT, NULL);

        for (int i = 0; i < n_event; i++)
        {
            int fd = events[i].ident;
            // ServerVec::iterator itS = this->getServClien(fd);
            if (events[i].flags & EV_EOF)
            {
                std::cout << "----EOF-----\n";
                keventUP(kq, fd,  EVFILT_READ , EV_CLEAR | EV_DELETE);
                std::cout <<"client "<< fd << " is disconnected\n";
                Cmap.erase(fd);
                close(fd);

                std::cout << "----END_EOF-----\n";
            }
            else if ( this->ifServer(fd) )
            { //new client
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
                if (sendData(kq, fd) < 0)
                    continue;
            }
        }
        this->Timeout(kq);
    }
}

void webServ::Timeout(int kq){
    for (_ClientMap::iterator it = this->Cmap.begin(); it != this->Cmap.end(); it++){
        if (std::time(NULL) - it->second.getTime() > 3)
        {
            it->second.setStep(-2);
        	keventUP(kq, it->first, EVFILT_READ, EV_DISABLE);
            keventUP(kq, it->first, EVFILT_WRITE, EV_CLEAR|EV_ENABLE | EV_ADD);
            // sendData(kq, it->first);
        }
    }

}



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
    Res rs(Servers[i].getConfig(), ServerSock, clientSock);
    std::cout << "waa zzeeeb\n";
    testConnection(clientSock, "accepte a new client");
    this->Cmap.insert(std::make_pair(clientSock, rs));
    keventUP(kq, clientSock, EVFILT_READ, EV_ADD);
    std::cout << "----Accepted new client connection on socket----\n";
    return 0;
}

/*_______SEND DATA___________*/
/*_______SEND DATA___________*/
/*_______SEND DATA___________*/

int webServ::sendData(int kq,int fd){
    std::cout << "\n------send data---------\n";
    int ServerFd = Cmap.find(fd)->second.getServerFd();
    size_t i = 0;
    for(; i < Servers.size(); i++){
        if (Servers[i].getSock() == ServerFd)
            break;;
    }
    std::cout << "server n = " << Servers[i].getSock() << "map size = " << this->Cmap.size() << "\n";
    Res &res = this->Cmap.find(fd)->second;
   res.buildResponse();
   size_t length = res.getResp().size();
   if (send(fd, res.getResp().data(), length, 0) < 0)
        std::cout << "SEND FAILD\n";
    keventUP(kq, fd, EVFILT_WRITE, EV_DISABLE);
	keventUP(kq, fd, EVFILT_READ, EV_CLEAR | EV_ENABLE | EV_ADD);
    std::cout << "SSSTTTEEEP = " << res.getStep() << "\n";
    if (res.getStep() == TIMEOUT)
    {
        keventUP(kq, fd, EVFILT_WRITE, EV_DELETE);
        keventUP(kq, fd, EVFILT_READ, EV_DELETE);
        Cmap.erase(fd);
        close(fd);
    }
    else
	    res.resetvalues();
    std::cout << "LLLLLLL \n";    std::cout << "------------\n";
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
    // memset(buffer, 0, 1024);
    int rd = recv(fd, buffer, event.data, 0);
    if (rd <= 0)
    {
        if (rd == 0)
            std::cout <<"client "<< fd << " is disconnected\n";
		else
			std::cout << "Error receving data from client\n";
        keventUP(kq, fd,  EVFILT_READ , EV_CLEAR | EV_DELETE);
        Cmap.erase(fd);
        return (-1);
    }
    else
    {
        // buffer[rd] = 0;
        vec = addtoVec(buffer, event.data);
        std::string req(vec.begin(), vec.end());

        //append the read string in the request class
        Cmap.find(fd)->second.append(req);
        std::cout << *dynamic_cast<Req *>(&Cmap.find(fd)->second) << std::endl;;
    }
    std::cout << "r = " << Cmap.find(fd)->second.getStep() << std::endl;;
    if (Cmap.find(fd)->second.getStep() > 2 || Cmap.find(fd)->second.getStep() < 0)
    {
        std::cout << "daz mn hna\n";
		keventUP(kq, fd, EVFILT_READ, EV_DISABLE);
        keventUP(kq, fd, EVFILT_WRITE, EV_CLEAR|EV_ENABLE | EV_ADD);
    }
    Cmap.find(fd)->second.updateTime();
    std::cout << "--------finish read data---------\n";
    return 0;
}

std::vector<Server> &webServ::getServers(){
    return this->Servers;
}

