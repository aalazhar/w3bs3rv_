#pragma once

#include "Headers.hpp"

struct loca{
    std::string root;
    std::string index;
    std::string autoIndex;
    std::string allowed_m;
    std::string deny;
    std::string redirect;
    std::string cgiExt;
    std::string cgiPath;
    std::string l_path;
    std::vector <std::string> a_meth;
    std::vector <std::string> cgi;
    int l_flag;

};

struct config{
    int port;
    std::string listen;
    std::string pRoot;
    std::string index;
    std::string autoIndex;
    std::string upload;
    std::string error_page;
    std::string allowed_m;
    std::vector <loca> vect;
    std::vector <std::string> err_p;
    std::vector <std::string> a_meth;
    std::vector <std::string> lsten;
    std::vector <std::string> uploads;
};

class Server{
    private:
        int port;
        int servFd; // socket fd
        struct config ServerConfig;
    public:
        

};