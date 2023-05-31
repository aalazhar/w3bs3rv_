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


/*
    ERROR STEPS in Req class:
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
        int chunkSize;
        struct config _Config;







    public:
        void clearData();
        Req(int, int, struct config &);
        void addTovect(std::string );
        void addTovect( const char *, size_t  );
        void setStep(int s){this->step = s;}
        Req &operator=(const Req &);
        void append(const std::string&);
        int getMETHOD(std::string&);
        std::string &getMETHOD();
        std::string &getURL();
        std::string &getHTTPV();
        std::string &getBody();
        int getStep(){return step;};
        int getServerFd(){return this->ServerFd;}
        int getClientFd(){return this->clientFd;}
        void parseErr(const int&);
        int parseHeaders(std::string&);
        int parseBody(std::string&);
        void parseCHuncked(std::string&);
        int checkMETHOD(const std::string&);
        _map getHEADERS();
        void updateTime();
        time_t getTime();
        void    printRe(){
            iter_map iter = HEADERS.begin();
            for (; iter != HEADERS.end(); iter++)
                std::cout << iter->first << " : " << iter->second << std::endl;
        }
        iter_map   reqBegin(){ return (HEADERS.begin());}
        iter_map   reqEnd(){ return (HEADERS.end()); }

        void checkSendType();
};

std::ostream &operator<<(std::ostream &, Req &);





/*

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>

int main() {
    int kq = kqueue();
    if (kq == -1) {
        // Handle error
        return 1;
    }

    struct kevent kev;
    struct timespec timeout;
    EV_SET(&kev, 0, EVFILT_TIMER, EV_ADD | EV_ENABLE, NOTE_SECONDS, 5, nullptr);
    timeout.tv_sec = 0;
    timeout.tv_nsec = 0;

    while (true) {
        struct kevent events;
        int nevents = kevent(kq, &kev, 1, &events, 1, &timeout);
        if (nevents == -1) {
            // Handle error
            break;
        } else if (nevents > 0) {
            if (events.filter == EVFILT_TIMER) {
                // Timer event occurred, handle timeout
                // Disconnect the client or perform any necessary action
                // ...
                break;
            }
        }
    }

    close(kq);

    return 0;
}


*/