#include "RequestClass.hpp"

Req::Req(){
    this->step = 0;
}


void Req::append(const std::string &rq){
    std::cout << "hello\n";
    std::string s;
    std::stringstream st(rq);
    while (getline(st, s, '\n') && step >= 0){
        if (step == 0)
            getMETHOD(s);
        else if (step == 1)
            parseHeaders(s);
        
        else if (step == 2){
            s += "\n";
            parseBody(s);
        }
    }

}

int Req::parseBody(std::string &s){
    if (METHOD != "POST")
        return step = 3;
    this->Body.append(s);
    size_t BodySize = (size_t)std::atoi(HEADERS["Content-Length"].c_str());
    if (Body.size() >= BodySize)
        return step = 3;
    return 0;
}

Req &Req::operator=(const Req &other){
    this->URL = other.URL;
    this->METHOD = other.METHOD;
    this->HTTPV = other.HTTPV;
    this->HEADERS = other.HEADERS;
    this->mimetypes = other.mimetypes;
    this->Body = other.Body;
    this->step = other.step;
    return *this;
}


// Req::Req(const std::string &req){
//     // int i = 0;
//     // std::string s;
//     // std::stringstream st(req);
//     // while (getline(st, s, '\n')){
//     //     if (i == 0){
//     //         i++;
//     //         if (this->getMETHOD(s) < 0)
//     //             parseErr(1);
//     //     }else if (i == 1){
//     //         i++;
//     //         if (parseHeaders(s) < 0)
//     //             parseErr(2);
//     //     }/*else{
//     //         if (parseBody(req) < 0)
//     //             parseErr(3);
//     //     }*/
//     // }
// }

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
        return this->step = 2;
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
    return this->step = -1;
}


std::string &Req::getURL(){
	return this->URL;
}
std::string &Req::getMETHOD(){
	return this->METHOD;
}
std::string &Req::getHTTPV(){
	return this->HTTPV;
}
std::string &Req::getBody(){
	return this->Body;
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
    //if (checkURL(s) < 0)
        // return -1;
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
        return this->step = -3;
    HTTPV = s;
    this->step = 1;
    return 0;
}

std::map<std::string, std::string> Req::getHEADERS(){
	return this->HEADERS;
}

std::ostream &operator<<(std::ostream &os, Req &request){
	os << "====step=" << request.getStep() << std::endl;
	os  << request.getMETHOD()<< " " << request.getURL() << " " << request.getHTTPV()  << std::endl;
	iter_map it = request.reqBegin();
	for(; it != request.reqEnd(); it++)
		os << it->first << ": " << it->second << std::endl;
	os << request.getBody() << std::endl;
	return os;
}