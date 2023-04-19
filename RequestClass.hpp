 #pragma once

#include <fstream>
#include <map>
#include <iostream>
#include <string>
#include <sstream> 
#include <vector>

/*
    ERROR STEPS:
    3 : parse body done 100 complete 
    2 : parse headers done
    1 : parse methode and URL and HTTPV done
    0 : void Request
    -1 : invalide method
    -2 : invalid URL
    -3 : invalid HTTPV

*/


#define CRLF "\r\n"
        typedef std::map<std::string, std::string> _map;
        typedef std::map<std::string, std::string>::iterator iter_map;

class Req{
    private:
        _map HEADERS;
        _map mimetypes;
        std::string METHOD;
        std::string URL;
        std::string HTTPV;
        std::string Body;
        int step;

    public:
        Req();
        // Req(const std::string&);
        Req &operator=(const Req &);
        void append(const std::string&);
        int getMETHOD(std::string&);
        std::string &getMETHOD();
        std::string &getURL();
        std::string &getHTTPV();
        std::string &getBody();
        int getStep(){return step;};
        void parseErr(const int&);
        int parseHeaders(std::string&);
        int parseBody(std::string&);

        int checkMETHOD(const std::string&);
        _map getHEADERS();
        
        void    printRe(){
            iter_map iter = HEADERS.begin();
            for (; iter != HEADERS.end(); iter++)
                std::cout << iter->first << " : " << iter->second << std::endl;
        }
        iter_map   reqBegin(){ return (HEADERS.begin());}
        iter_map   reqEnd(){ return (HEADERS.end()); }
};