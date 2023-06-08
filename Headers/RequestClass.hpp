#pragma once

#include "Headers.hpp"
#include "parserObjectU.hpp"

#define DONE 4
#define CGII 10
#define BINARYFILE 11
#define NORMFILE 12
#define ERROR 13
#define CHUNCKED 3
#define CHUNCKEDDONE 30
#define TIMEOUT -2
#define BADREQ -1


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
        std::vector<char> body;
        time_t time;
        int step;
        int ServerFd;
        int clientFd;
        long chunkSize;
        long var;
        struct config _Config;

    public:
        Req(int, int, struct config &);
        void creatfile();
        void check();
        void addTovect(std::string );
        void addTovect( const char *, size_t  );
        void setStep(int s);
        Req &operator=(const Req &);
        void append(const std::string&);
        void parseErr(const int&);
        int parseHeaders(std::string&);
        int parseBody(std::string&);
        void parseCHuncked(std::string&);
        int checkMETHOD(const std::string&);

        int getStep();
        int getServerFd();
        int getClientFd();
        int getMETHOD(std::string&);
        std::string &getMETHOD();
        std::string &getURL();
        std::string &getHTTPV();
        std::string &getBody();
        _map getHEADERS();
        void updateTime();
        time_t getTime();
        void checkSendType();
        void clearData();
};
