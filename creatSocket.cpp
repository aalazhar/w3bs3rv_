#include "creatSocket.hpp"

creatSocket::creatSocket(int domain, int Socktype, int protocol, int port, unsigned int interface){
	memset((char*)&this->Addr, 0, sizeof(this->Addr));
    this->Addr.sin_family = (sa_family_t)domain;
    this->Addr.sin_port = htons(port);
    this->Addr.sin_addr.s_addr = htonl(interface);
    this->Addr.sin_len = sizeof(Addr);
	//create a new socket
	/*domain is PF_INET or PF_INET6, ip4 || ip6
      type is SOCK_STREAM or SOCK_DGRAM,
      protocol can be set to 0 to choose the proper protocol for the given type*/
    this->sockFd = socket(domain, Socktype, protocol);
	testConnection(this->sockFd, "create Socket");
	//binding and listning:
	BindAndListenSocket();

}

void creatSocket::testConnection(const int& test, const std::string& msg){
    if (test < 0){
        std::cerr << "Faild to connection ?!: " << msg << std::endl;
		std::cerr << "Error : " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
    }
}

const sockaddr_in &creatSocket::getSockAddr(){ return this->Addr; }
const int &creatSocket::getSockFd(){ return this->sockFd; }
const int &creatSocket::getlistning(){ return this->listning; }
const int &creatSocket::getBinding(){ return this->binding; }

void creatSocket::BindAndListenSocket(){
	int yes = 1;
	int setsock = setsockopt(this->sockFd, SOL_SOCKET,SO_REUSEPORT, &yes, sizeof(yes));
	testConnection(setsock, "Error setsockopt() option");
	this->binding = bind(this->sockFd, (struct sockaddr *)&this->Addr, this->Addr.sin_len);
	testConnection(this->binding, "binding the Socket");
	this->listning = listen(this->sockFd, BACKLOG);
	testConnection(this->listning, "Listen the Socket");
	//create a kqueue
	this->kq = kqueue();
	testConnection(this->kq, "create a kqueue");
	//initialize the kevent structure
    EV_SET(&evSet, this->sockFd, EVFILT_READ, EV_ADD, 0, 0, NULL);
	//monitory the socket_fd
    testConnection(kevent(kq, &evSet, 1, NULL, 0, NULL), "monitory the socket_fd");

}

void creatSocket::launch(){
	int client_sock;
	while(true){
		std::cout << "=====WAITING=====\n";
		
		struct kevent events[MAXEVENT];
		int n_event = kevent(kq, NULL, 0, events, MAXEVENT, NULL);
		testConnection(n_event, "waiting for events");
		for (int i = 0; i < n_event; i++){
			int fd = events[i].ident;
			if (fd == this->sockFd){
				//accept a new client
				struct sockaddr_in client_addr;
				socklen_t client_addr_len = sizeof(client_addr);
                client_sock = accept(this->sockFd, (struct sockaddr*)&client_addr, &client_addr_len);
				testConnection(client_sock, "accepte a new client");

				//add the new client socket to the kqueue
				struct kevent evSet;
				EV_SET(&evSet, client_sock, EVFILT_READ, EV_ADD, 0, 0, NULL);
				if (kevent(kq, &evSet, 1, NULL, 0, NULL) < 0){std::cerr << "adding client socket to kqueue\n";close(client_sock); continue;}
				clients.push_back(client_sock);
				std::cout << "Accepted new client connection on socket\n";
			}else{
				// Handle incoming data from a client
				char buffer[1024];
				int rd = recv(fd, buffer, sizeof(buffer), 0);
				if (rd <= 0){
					if (rd == 0)
						std::cout <<"client is disconnected\n";
					else
						std::cout << "Error receving data from client\n";
					close(fd);
					for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); ++it) {
                        if (*it == fd) {
                            clients.erase(it);
                            break;
                        }
                    }
				} else {
					buffer[rd] = 0;
					std::string req(buffer); 
					// Req request(req);
					std::cout << req << std::endl;
					std::string hello = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><html><head><title>Page Title</title></head><body><h1>Hello</h1><p>m3a Mhooom</p></body></html>";
					send(client_sock, hello.c_str(), hello.length(), 0);
					close(client_sock);
				}
			}
		}



		std::cout << "=====DONE=====\n";
		// sleep(5);
	}
}




// void creatSocket::launch(){
// 	while(true){
// 		std::cout << "=====WAITING=====\n";

// 		this->nwSock = accept(this->sockFd, (struct sockaddr *)&Addr, (socklen_t *)&Addr.sin_len);
		
		
// 		char r[1000];
// 		int read = recv(nwSock,r,sizeof(r),0 );
// 		if (read == 0)
// 			std::cout << "client is disconnected" << std::endl;
// 		if (read < 0)
// 			std::cout << "error" << std::endl;
// 		if (read > 0)
// 		{
// 			r[read] = 0;
// 			std::string req(r); 
// 			std::cout << req << std::endl;
// 			// sleep(5);
// 			// request.printRe();
// 		}
// 		// std::ifstream in(this->nwSock);
// 		// std::string data;
// 		// in >> data;


// 		std::string hello = "<!DOCTYPE html><html><head><title>Page Title</title></head><body><h1>Hello</h1><p>m3a Mhooom</p></body></html>";
// 		send(nwSock, hello.c_str(), hello.length(), 0);
// 		close(nwSock);

// 		std::cout << "=====DONE=====\n";
// 		// sleep(5);
// 	}
// }

