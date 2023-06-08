#include "../Headers/Server.hpp"

#define BACKLOG 10

Server::Server(struct config &c): ServerConfig(c){
    std::string port = c.lsten[1];
    this->port = std::stoi(port);
}

int Server::creatSocket(){
	memset((char*)&this->Addr, 0, sizeof(this->Addr));
    this->Addr.sin_family = (sa_family_t)AF_INET;
    this->Addr.sin_port = htons(this->port);
    this->Addr.sin_addr.s_addr = inet_addr(ServerConfig.lsten[0].c_str());
    this->Addr.sin_len = sizeof(Addr);
    this->sockFd = socket(this->Addr.sin_family, SOCK_STREAM,0);
    testConnection(this->sockFd, "creat Socket");

    int yes = 1;
	int setsock = setsockopt(this->sockFd, SOL_SOCKET,SO_REUSEPORT, &yes, sizeof(yes));
	testConnection(setsock, "Error setsockopt() option");
	

	testConnection( bind (this->sockFd, (struct sockaddr *)&this->Addr, this->Addr.sin_len), "binding the Socket");
	
	testConnection(listen(this->sockFd, BACKLOG), "Listen the Socket");
    return this->sockFd;
}

void Server::testConnection(const int& test, const std::string& msg){
    if (test < 0){
        std::cerr << "Faild to connection ?!: " << msg << std::endl;
		std::cerr << "Error : " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
    }
}

struct config &Server::getConfig(){
    return this->ServerConfig;
}

int Server::getSock(){
    return this->sockFd;
}

sockaddr_in Server::getAddr(){return this->Addr;}

int Server::getport() {return this->port;}