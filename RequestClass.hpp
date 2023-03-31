#pragma once

#include <fstream>
#include <map>
#include <iostream>
#include <string>
#include <sstream> 
#include <vector>

class Req{
    private:
        std::map<std::string, std::string> req;
        std::map<std::string, std::string> mimetypes;
        std::string METHOD;
        std::string URL;
        std::string HTTPV;

        Req();
    public:
        typedef std::map<std::string, std::string> req_type;
        typedef std::map<std::string, std::string>::iterator iter_type;
        Req(const std::string&);
        int getMETHOD(std::string&);
        void parseErr(const int&);
        int parseHeaders(std::string&);
        int checkMETHOD(const std::string&);
        std::map<std::string, std::string>getRequest();
        void    printRe(){
            iter_type iter = req.begin();
            for (; iter != req.end(); iter++)
                std::cout << iter->first << std::endl;
        }
        iter_type   reqBegin(){
            return (req.begin());
        }
        iter_type   reqEnd(){
            return (req.end());
        }
};