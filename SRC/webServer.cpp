#include "../Headers/webServer.hpp"
#define MAXEVENT 16
void ft_err(){
    exit(1);
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

    int a = 0;
    int kq = kqueue();
    struct kevent evSet;
    // std::map<int, Server>::iterator it = getItbegin();
    for (ServerMap::iterator it = getItbegin(); it != getItend(); it++){
        EV_SET(&evSet, it->first, EVFILT_READ, EV_ADD, 0, 0, NULL);
        kevent(kq, &evSet, 1, NULL, 0, NULL);
    }
    int clientSock;
	std::vector<Req *> ArrReq;
    while (true){
        std::cout << "=====WAITING=====\n";

		struct kevent events[MAXEVENT];
		int n_event = kevent(kq, NULL, 0, events, MAXEVENT, NULL);
        //if n < 0
        for (int i = 0; i < n_event; i++)
        {
            int fd = events[i].ident;
            ServerMap::iterator itS = this->_mySrvs.find(fd);
            if ( itS != this->getItend() )
            {
                //new connection
                struct sockaddr_in client_addr;
				socklen_t client_addr_len = sizeof(client_addr);
                clientSock = accept(itS->first, (struct sockaddr*)&client_addr, &client_addr_len);
                testConnection(clientSock, "accepte a new client");
                Req *req  = new Req();
                // Req req;
				//add the new client socket to the kqueue
				struct kevent evSet;
				EV_SET(&evSet, clientSock, EVFILT_READ, EV_ADD, 0, 0, NULL);
				// EV_SET(&evSet, clientSock, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
				if (kevent(kq, &evSet, 1, NULL, 0, NULL) < 0){std::cerr << "adding client socket to kqueue\n";close(clientSock) ; continue;}
                itS->second.getClientMap().insert(std::pair<int, Req *>(clientSock, req));

                std::cout << "Accepted new client connection on socket\n";
            }
            else
            {
                itS = getServClien(fd);
                char buffer[events[i].data];
                std::cout << "size : " << events[i].data << std::endl;
                memset(buffer, 0, events[i].data);
                int rd = recv(fd, buffer, events[i].data, 0);
                if (rd <= 0)
                {
                    if (rd == 0)
                        std::cout <<"client "<< fd << " is disconnected\n";
					else
						std::cout << "Error receving data from client\n";
					close(fd);
					//erase client  from _map
                    _ClientMap::iterator it2 = itS->second.getClientBegin(); 
					for (; it2 != itS->second.getClientEnd(); it2++) {
                        if (it2->first == fd)
                        {
                            std::cout << "HELLO\n";
                            for (std::vector<Req*>::iterator i = ArrReq.begin(); i != ArrReq.end(); i++)
                            {
                                if (*i == it2->second)
                                    ArrReq.erase(i);
                                    break;
                            }
                            delete it2->second;
                            itS->second.getClientMap().erase(it2);
                            break;
                        }
                    }
                }
                else{
                    buffer[events[i].data] = 0;
                    std::string req(buffer); 
                    std::cout << "*******\n" << req << "\n*******\n";
                    _ClientMap &m = itS->second.getClientMap();
                    m[fd]->append(req);
                    std::cout << m[fd] << std::endl;
                }
            }
            a = send(fd, "Hello", 6, 0);
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


