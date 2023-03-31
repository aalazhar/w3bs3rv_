#include "creatSocket.hpp"

creatSocket::creatSocket(int domain, int Socktype, int protocol, int port, unsigned int interface){
	/*domain is PF_INET or PF_INET6, ip4 || ip6
      type is SOCK_STREAM or SOCK_DGRAM,
      protocol can be set to 0 to choose the proper protocol for the given type*/
	memset((char*)&this->Addr, 0, sizeof(this->Addr));
    this->Addr.sin_family = (sa_family_t)domain;
    this->Addr.sin_port = htons(port);
    this->Addr.sin_addr.s_addr = htonl(interface);
    this->Addr.sin_len = sizeof(Addr);
    this->sockFd = socket(domain, Socktype, protocol);
    //check if sockFd < 0
	testConnection(this->sockFd, "creat Socket");
	//binding and listning:
	BindAndListenSocket();
}

void creatSocket::testConnection(const int& test, const std::string& msg){
    if (test < 0){
        std::cerr << "Faild to connection ?! Err From : " << msg << std::endl;
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
	testConnection(setsock, "Error setting socket option");
	this->binding = bind(this->sockFd, (struct sockaddr *)&this->Addr, this->Addr.sin_len);
	testConnection(this->binding, "binding the Socket");
	this->listning = listen(this->sockFd, BACKLOG);
	testConnection(this->listning, "Listen the Socket");

}

void creatSocket::launch(){
	while(true){
		std::cout << "=====WAITING=====\n";
		this->nwSock = accept(this->sockFd, (struct sockaddr *)&Addr, (socklen_t *)&Addr.sin_len);
		
		
		char r[1000];
		int read = recv(nwSock,r,sizeof(r),0 );
		if (read == 0)
			std::cout << "client is disconnected" << std::endl;
		if (read < 0)
			std::cout << "error" << std::endl;
		if (read > 0)
		{
			r[read] = 0;
			std::string req(r); 
			std::cout << req << std::endl;
			// sleep(5);
			// request.printRe();
		}
		// std::ifstream in(this->nwSock);
		// std::string data;
		// in >> data;


		std::string hello = "<!DOCTYPE html><html><head><title>Page Title</title></head><body><h1>Hello</h1><p>m3a Mhooom</p></body></html>";
		send(nwSock, hello.c_str(), hello.length(), 0);
		close(nwSock);

		std::cout << "=====DONE=====\n";
		// sleep(5);
	}
}

