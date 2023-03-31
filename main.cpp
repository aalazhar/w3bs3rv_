#include "creatSocket.hpp"

int main(){
    creatSocket sock(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY);
    sock.launch();
}