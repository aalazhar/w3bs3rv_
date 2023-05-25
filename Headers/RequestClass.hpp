 #pragma once

#include "Headers.hpp"

#define DONE 3

/*
    ERROR STEPS in Req class:
    3 : parse body done 100 complete 
    2 : parse headers done
    1 : parse methode and URL and HTTPV done
    0 : void Request
    -1 : invalide method
    -2 : TIMEOUT
    -3 : invalid HTTPV

*/


#define CRLF "\r\n"
typedef std::map<std::string, std::string> _map;
typedef std::map<std::string, std::string>::iterator iter_map;

class Req{
    protected:
        _map HEADERS;
        std::string METHOD;
        std::string URL;
        std::string HTTPV;
        std::string Body;
        time_t time;
        int step;
        int ServerFd;
        int clientFd;

    public:
        void clear(){
            this->HEADERS.clear();
            this->METHOD = "";
            this->URL = "";
            this->HTTPV = "";
            this->Body = "";
            this->step = 0;
        };
        Req(int, int);
        Req &operator=(const Req &);


        // Req(const std::string&);
        int getMETHOD(std::string&);
        std::string &getMETHOD();
        std::string &getURL();
        std::string &getHTTPV();
        std::string &getBody();
        int getStep(){return step;};
        int getServerFd(){return this->ServerFd;}
        time_t getTime();
        _map getHEADERS();

        void setStep(int s){
            this->step = s;
        }

        void append(const std::string&);
        void parseErr(const int&);
        int parseHeaders(std::string&);
        int parseBody(std::string&);
        int checkStep();
        int checkMETHOD(const std::string&);
        void updateTime();
        void    printRe(){
            iter_map iter = HEADERS.begin();
            for (; iter != HEADERS.end(); iter++)
                std::cout << iter->first << " : " << iter->second << std::endl;
        }
        iter_map   reqBegin(){ return (HEADERS.begin());}
        iter_map   reqEnd(){ return (HEADERS.end()); }
};

std::ostream &operator<<(std::ostream &, Req &);
