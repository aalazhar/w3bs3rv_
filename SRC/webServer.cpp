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
	std::vector<Req *> ArrReq;
    while (true){
        struct timespec timeout;
        timeout.tv_sec = TIMEOUT;
        timeout.tv_nsec = 0;
		struct kevent events[MAXEVENT];
		int n_event = kevent(kq, NULL, 0, events, MAXEVENT, &timeout);
        //if n < 0
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
                if (readData(kq ,fd, itS, events[i]) < 0)
                    continue;;
            } 
            // else if (events[i].filter == EVFILT_WRITE)
            // {
            //     if (sendData() < 0)
            //         continue;
            // }
        }
        for (ServerMap::iterator it = getItbegin(); it != getItend(); it++){

            for (_ClientMap::iterator it2 = it->second.getClientBegin(); it2 != it->second.getClientEnd(); ++it2) {
                if (it2->second->getStep() < 0 || it2->second->getStep() == 3){
                    ArrReq.push_back(it2->second);
                }
            }
        }

        size_t j = 0;
        std::vector<Req*>::iterator i = ArrReq.begin();
        std::cout << "REQ SIZE = " << ArrReq.size() << std::endl;
        for (;i != ArrReq.end() && j < ArrReq.size();i++, j++){
            std::cout << "-----ADDR : " << *i <<" ------------\n" << **(i) << "||\n+++++++++++++++++++" << std::endl;
            ArrReq.erase(i);
        }
		std::cout << "=====DONE=====\n";
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
    Req *req  = new Req(); // delete mli ysaaali program

	//add the new client socket to the kqueue
	struct kevent evSet;
	EV_SET(&evSet, clientSock, EVFILT_READ, EV_ADD, 0, 0, NULL);
	if (kevent(kq, &evSet, 1, NULL, 0, NULL) < 0)
        return std::cerr << "adding client socket to kqueue\n", close(clientSock), -1;
    Server->second.getClientMap().insert(std::pair<int, Req *>(clientSock, req));

    std::cout << "Accepted new client connection on socket\n";
    return 0;
}


int webServ::readData(int &kq, int& fd, ServerMap::iterator &Server, struct kevent &event){
    Server = getServClien(fd);
    char buffer[event.data];
    _ClientMap m;
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
        m = Server->second.getClientMap();
        m[fd]->append(req);
        std::cout << "*******\n" << m[fd] << "\n*******\n";
        
    }
    if (m[fd]->getStep() == 3)
    {
        keventUP(kq, fd, EVFILT_WRITE, EV_CLEAR|EV_ENABLE | EV_ADD);
		keventUP(kq, fd, EVFILT_READ, EV_DISABLE);
    }
    return 0;
}
