#include "../Headers/Headers.hpp"
#include "../Headers/webServer.hpp"



int main(int ac, char **av){
    if (ac != 2)
        return std::cerr << "ERROR PARAM\n", 1;
    try{
        std::string arg(av[1]);
        webServ webserver(arg);
        webserver.lunche();
    }catch(const std::exception &ex){
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}