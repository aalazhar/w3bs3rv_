#include "../Headers/webServer.hpp"
#define MAXEVENT 64
void ft_err(){
    exit(1);
}

void webServ::keventUP(int kq, int fd, int filter, int flag){
    struct kevent ev;
    EV_SET(&ev, fd, filter, flag, 0, 0, NULL);
    testConnection (kevent(kq, &ev, 1, NULL, 0, NULL), "FAILDE ADD THE FD TO KEVENT");
}

int checkConfig(config &c){
    (void)c;
    return 0;
}

void checkObj(parserObject &obj){
    std::vector<config>::iterator it = obj.getItBegin();
    while (it != obj.getItend()){
        if (checkConfig(*it) < 0)
            ft_err();
        it++;
    }

}


void webServ::creatServers(parserObject &obj){
    std::vector<config>::iterator it = obj.getItBegin();
    for (;it != obj.getItend(); it++){
        Server s(*it);
        this->_mySrvs.insert(std::pair<int, Server>(s.creatSocket(),s));
    }
}


webServ::webServ(const std::string &filename){
    parserObject obj(filename);
    obj.open_config_file();
    checkObj(obj);
    creatServers(obj);
}




void webServ::lunche(){
    int kq = kqueue();
    for (ServerMap::iterator it = getItbegin(); it != getItend(); it++)
        keventUP(kq, it->first, EVFILT_READ, EV_ADD);
    
    int clientSock = 0;
	std::vector<Response *> ArrReq;
    while (true){
        struct timespec timeout;
        timeout.tv_sec = TIMEOUT;
        timeout.tv_nsec = 0;
		struct kevent events[MAXEVENT];
		int n_event = kevent(kq, NULL, 0, events, MAXEVENT, &timeout);
        for (int i = 0; i < n_event; i++)
        {
            int fd = events[i].ident;
            ServerMap::iterator itS = this->_mySrvs.find(fd);
            if (events[i].flags & EV_EOF)
            {
                keventUP(kq, fd,  EVFILT_READ , EV_CLEAR | EV_DELETE);
                std::cout <<"client "<< fd << " is disconnected\n";
                close(fd);
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
        // this->Timeout();
    }

}




void webServ::testConnection(const int& test, const std::string& msg){
    if (test < 0){
        std::cerr << "Faild to connection ?!: " << msg << std::endl;
		std::cerr << "Error : " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
    }
}


int webServ::acceptNewCl(int kq, int& clientSock, ServerMap::iterator &Server){
    struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
    clientSock = accept(Server->first, (struct sockaddr*)&client_addr, &client_addr_len);
    testConnection(clientSock, "accepte a new client");
    Response *res  = new Response(Server->second.getConfig()); // delete mli ysaaali program

	//add the new client socket to the kqueue
	struct kevent evSet;
	EV_SET(&evSet, clientSock, EVFILT_READ, EV_ADD, 0, 0, NULL);
	if (kevent(kq, &evSet, 1, NULL, 0, NULL) < 0)
        return std::cerr << "adding client socket to kqueue\n", close(clientSock), -1;
    Server->second.getClientMap().insert(std::pair<int, Response *>(clientSock, res));

    std::cout << "Accepted new client connection on socket\n";
    return 0;
}

int webServ::sendData(int &kq,int& fd, struct kevent &event){
    ServerMap::iterator Server = getServClien(fd);
    _ClientMap _clientMap = Server->second.getClientMap();
    Response *res = _clientMap[fd];
    if (res->getR() == 0)
        res->makeResponse();
    // char *str[event.data];
    std::string response = res->getStatusLine() + CRLF + res->getheaders() + CRLF + res->getResponse_body();
    std::cout << "---RESPONS---\n" << response << std::endl;
    
    int length = event.data;
    const char *buff;
    if (res->getR() < response.length())
        buff = &response.c_str()[res->getR()];
    if (send(fd, response.c_str(), length, 0) < 0)
        std::cout << "---------------\n";
    res->setR(res->getR() + length);
    if (res->getR() > response.length())
    {
        delete res;
        keventUP(kq, fd, EVFILT_WRITE, EV_DISABLE);
        keventUP(kq, fd, EVFILT_READ, EV_CLEAR | EV_ENABLE | EV_ADD);
        _clientMap.erase(fd);
    }
    return 0;
    //i need the size of the response , to send the buffer

}

int webServ::readData(int &kq, int& fd, struct kevent &event){
    ServerMap::iterator Server = getServClien(fd);
    char buffer[event.data];
    // _ClientMap m;
    std::cout << "size : " << event.data << std::endl;
    memset(buffer, 0, event.data);
    int rd = recv(fd, buffer, event.data, 0);
    if (rd <= 0)
    {
        if (rd == 0)
            std::cout <<"client "<< fd << " is disconnected\n";
		else
			std::cout << "Error receving data from client\n";
		close(fd);
		//erase client  from _map
        _ClientMap::iterator it2 = Server->second.getClientBegin(); 
		for (; it2 != Server->second.getClientEnd(); it2++) {
            if (it2->first == fd)
            {
                delete it2->second;
                Server->second.getClientMap().erase(it2);
                break;
            }
        }
    }
    else
    {
        buffer[event.data] = 0;
        std::string req(buffer); 
        //append the read string in the request class
        Server->second.getClientMap()[fd]->append(req);
        // std::cout << "*******\n" << Server->second.getClientMap()[fd] << "\n*******\n";
        
    }
    if (Server->second.getClientMap()[fd]->getStep() == DONE)
    {
		keventUP(kq, fd, EVFILT_READ, EV_DISABLE);
        keventUP(kq, fd, EVFILT_WRITE, EV_CLEAR|EV_ENABLE | EV_ADD);
    }
    return 0;
}
