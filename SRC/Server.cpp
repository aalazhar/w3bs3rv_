#include "../Headers/Server.hpp"

#define BACKLOG 10

Server::Server(struct config &c): ServerConfig(c){
    std::string port = c.lsten[1];
    this->port = std::stoi(port);;
}

int Server::creatSocket(){
    int yes = 1;
	memset((char*)&this->Addr, 0, sizeof(this->Addr));
    this->Addr.sin_family = (sa_family_t)AF_INET;
    this->Addr.sin_port = htons(this->port);
    this->Addr.sin_addr.s_addr = inet_addr(ServerConfig.lsten[0].c_str());
    this->Addr.sin_len = sizeof(Addr);

    ErrorHandler(this->sockFd = socket(this->Addr.sin_family, SOCK_STREAM,0), "FAILD");
	ErrorHandler(setsockopt(this->sockFd, SOL_SOCKET,SO_REUSEPORT, &yes, sizeof(yes)), "FAILD setsockopt() option");
	ErrorHandler( bind (this->sockFd, (struct sockaddr *)&this->Addr, this->Addr.sin_len), "FAILD TO BIND" );
	ErrorHandler( listen(this->sockFd, BACKLOG), "FAILD TO LISTEN" );
    ErrorHandler( fcntl(this->sockFd, F_SETFL, O_NONBLOCK) , "FAILD TO SET UP NONBLOCKIN FLAG" );

    return this->sockFd;
}

void Server::ErrorHandler(const int test, const std::string msg){
    if (test < 0){
        std::cerr << msg << std::endl;
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