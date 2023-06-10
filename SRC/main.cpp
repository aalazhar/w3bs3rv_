#include "../Headers/Headers.hpp"
#include "../Headers/webServer.hpp"

int main(int ac, char **av){
    if (ac != 2 and ac != 1)
        return std::cerr << "ERROR PARAM\n", 1;
    try{
        std::string arg;
        if (ac == 2)
            arg = av[1];
        else
            arg = "Config/default.config";
        webServ webserver(arg);
        webserver.lunche();
    }catch(const std::exception &ex){
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}