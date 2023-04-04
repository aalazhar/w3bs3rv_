#include "RequestClass.hpp"

Req::Req(const std::string &req){
    int i = 0;
    std::string s;
    std::stringstream st(req);
    while (getline(st, s, '\n')){
        if (i == 0){
            i++;
            if (this->getMETHOD(s) < 0)
                parseErr(1);
        }else if (i == 1){
            i++;
            if (parseHeaders(s) < 0)
                parseErr(2);
        }/*else{
            if (parseBody(req) < 0)
                parseErr(3);
        }*/
    }
}

// int Req::parseBody(const std::string &req){
//     size_t pos = req.find("\r\n\r\n", 0);
//     if (pos == std::string::npos)
//         return -1;
//     if ()
    
    
//     return 0;
// }

int Req::parseHeaders(std::string&hd){
    std::string key;
    std::string value;
    std::string::iterator i = hd.begin();
    std::string::iterator j = hd.begin();

    if (hd == "\r")
        return 0;
    while (i != hd.end() && *i != ':')
        i++;

    if (i == hd.end()&& *i != '\r')
        return -2;
    while(j != i){
        key.push_back(*j);
        j++;
    }
    i = i + 2;
    while (i != hd.end() && *i != '\r'){
        value.push_back(*i);
        i++;
    }
    // this->req.insert({key, value});
    std::cout << "key :" << key <<std::endl;
    std::cout << "value :" << value <<std::endl;
    HEADERS[key]  = value;
    HEADERS.insert(std::pair<std::string, std::string>(key, value));
    return 0;
}

void Req::parseErr(const int&i){
    (void) i;
    std::cerr << "ERRRRRROOOOOOORRRRRR\n";
}

int Req::checkMETHOD(const std::string &method){
    if (method == "GET" || method == "POST" || method == "DELETE")
        return 0;
    return -1;
}


int Req::getMETHOD(std::string &meth){
    std::string s;
    std::string::iterator i = meth.begin();
    std::string::iterator j = meth.begin();
    /*____-check-METHODE-_____*/
    while (i != meth.end() && *i != ' ')
        i++;
    while (j != i){
        s.push_back(*j);
        j++;
    }
    j = ++i;
    if (checkMETHOD(s) < 0)
        return (-1);
    METHOD = s;
    /*____-check-URL-______*/
    s.erase(s.begin(), s.end());
    while (i != meth.end() && *i != ' ')
        i++;
    while (j != i){
        s.push_back(*j);
        j++;
    }
    j = ++i;
    URL = s;
    /*____-check-HTTPV-_____*/
    s.erase(s.begin(), s.end());
    while (i != meth.end() && *i != '\r')
        i++;
    if (*i != '\r')
        return -1;
    while (j != i){
        s.push_back(*j);
        j++;
    }if (s != "HTTP/1.1")
        return -1;
    HTTPV = s;
    std::cout << "getMETHods:: " << std::endl;
    std::cout << "  METHODE = " <<  METHOD << std::endl;
    std::cout << "  URL = " <<  URL << std::endl;
    std::cout << "  HTTPV = " <<  HTTPV << std::endl;
    return 0;
}

std::map<std::string, std::string> Req::getHEADERS(){
	return this->HEADERS;
}