 #pragma once

#include <fstream>
#include <map>
#include <iostream>
#include <string>
#include <sstream> 
#include <vector>

class Req{
        typedef std::map<std::string, std::string> _map;
        typedef std::map<std::string, std::string>::iterator iter_map;
    private:
        _map HEADERS;
        _map mimetypes;
        std::string METHOD;
        std::string URL;
        std::string HTTPV;
        std::string Body;

        Req();
    public:
        Req(const std::string&);
        int getMETHOD(std::string&);
        void parseErr(const int&);
        int parseHeaders(std::string&);
        // int parseBody(const std::string&);
        int checkMETHOD(const std::string&);
        _map getHEADERS();
        void    printRe(){
            iter_map iter = HEADERS.begin();
            for (; iter != HEADERS.end(); iter++)
                std::cout << iter->first << std::endl;
        }
        iter_map   reqBegin(){ return (HEADERS.begin());}
        iter_map   reqEnd(){ return (HEADERS.end()); }
};