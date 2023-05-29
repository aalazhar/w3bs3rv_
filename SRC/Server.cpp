#include "../Headers/Server.hpp"

#define BACKLOG 10


// int Server::getport(struct config &c){
//     std::string port = c.lsten[1];
//     return std::stoi(port);
// }

Server::Server(struct config &c): ServerConfig(c){
    std::string port = c.lsten[1];
    this->port = std::stoi(port);
    // std::cout << "size : " << Cmap.size() << "\n";
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

// void Server::printIt(){
//     _ClientMap::iterator it = Cmap.begin();
//     for(; it != Cmap.end(); it++){
//         std::cout << "Server[" << sockFd << "] == client fd = " << it->first <<std::endl;
//         std::cout << it->second << std::endl;
//     }
// }

// void Server::eraseClient(int fd){
//     std::cout <<  this->Cmap.size() << std::endl;
//     _ClientMap::iterator it = this->Cmap.begin();
//     std::cout <<  it->first << " iikhan "<< std::endl;
//     std::cout << (it != this->Cmap.end()) << "    aaaaaa\n";
//             // std::cout << "kgvkhskghskjghbbse hb\n"<< *it->second;
//     if (it != this->Cmap.end()){
//         Cmap.erase(it);
//     }
//     close(fd);
        
// }

// void Server::addNewClient(int fd){
//     if (Cmap.find(fd) == Cmap.end())
//         std::cout << "tkhwiira\n";
//     Response res(this->ServerConfig, this->sockFd, fd);
//     std::cout << "size of cmap = " << Cmap.size() << std::endl;;
//     // try{
//         this->Cmap.insert(std::make_pair(fd, res));
//         this->Clientvec.push_back(res);
//     // }
//     // catch(...){
//     //     std::cout << "ERRRRRRRRRROOOOOORRRR\n";
//     // }
// }

struct config &Server::getConfig(){
    return this->ServerConfig;
}

// _ClientMap::iterator Server::getClientBegin() {
//     return this->Cmap.begin();
// }

// _ClientMap::iterator Server::getClientEnd(){
//     return this->Cmap.end();
// }

// _ClientMap &Server::getClientMap(){
//     return  this->Cmap;
// }

int Server::getSock(){
    return this->sockFd;
}