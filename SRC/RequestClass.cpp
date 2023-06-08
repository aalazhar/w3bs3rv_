#include "../Headers/RequestClass.hpp"

Req::Req(int serverfd, int clientfd, struct config &serverConf) : _Config(serverConf) {
    this->step = 0;
    // this->_Config = serverConf;
    this->ServerFd = serverfd;
    this->clientFd = clientfd;
    this->chunkSize = -1;
    this->var = 0;
    this->updateTime();
}


void Req::append(const std::string &rq){
    std::string s;
    std::stringstream st(rq);
    while (getline(st, s, '\n') && step >= 0){
        if (step == 0)
            getMETHOD(s);
        else if (step == 1)
            parseHeaders(s);
        
        else if (step == 2){
            if (!st.eof())
                s += "\n";
            parseBody(s);
        }else if (step == CHUNCKED){
            if (!st.eof())
                s += "\n";
            parseCHuncked(s);
        }
    }
    this->checkSendType();
}

time_t Req::getTime(){ return this->time; }

void Req::updateTime(){
    this->time = std::time(NULL);
}

void Req::addTovect(std::string s){
    for (size_t i = 0; i < s.size();i++)
        body.push_back(s[i]);
}

void Req::addTovect(const char *s, size_t length){
    for (size_t i = 0; i < length;i++)
        body.push_back(s[i]);
}

void Req::creatfile(){
    int fd = open("hello.txt", O_CREAT | O_RDWR );
    if (fd < 0)
        return (void)(std::cout << "FAILE TO OPEN THE FILE" << std::endl );
    std::string::iterator it = this->Body.begin();
    long i = 0;
    while (it != this->Body.end())
    {
        i++;
        char c = *it;
        write(fd, &c, 1);
        it++;
    }
}

int Req::parseBody(std::string &s){
    this->Body.append(s);
    if (this->METHOD != "POST")
        return this->step = DONE;
    size_t BodySize = 0;
    if (HEADERS.find("Content-Length") != HEADERS.end())
        BodySize = (size_t)std::atoi(HEADERS["Content-Length"].c_str());
    if (Body.size() >= BodySize)
        return step = DONE;
    return 0;
}

int hexToDec(const std::string& hexString) {
    std::stringstream ss;
    ss << std::hex << hexString;  // Set the stringstream to interpret input as hexadecimal
    int decimalValue;
    ss >> decimalValue;  // Read the hexadecimal value as decimal
    return decimalValue;
}

void Req::parseCHuncked(std::string &s){
    if (this->chunkSize == -1 and this->var == 0 and s != "\r\n")
        return (void)(this->chunkSize = hexToDec(s));
    if (this->chunkSize == 0)
        return (void)(this->step = CHUNCKEDDONE);
    for (int i = 0; this->var < chunkSize && i < static_cast<int>(s.size()); i++, this->var++){
        this->Body.push_back(s[i]);
    }
    if (this->var >= this->chunkSize)
        return (void)(this->chunkSize = -1, this->var = 0);
}

Req &Req::operator=(const Req &other){
    this->URL = other.URL;
    this->METHOD = other.METHOD;
    this->HTTPV = other.HTTPV;
    this->HEADERS = other.HEADERS;
    this->Body = other.Body;
    this->step = other.step;
    return *this;
}

void Req::checkSendType(){
    
    size_t  pos = this->URL.rfind(".");
    std::string type = this->URL.substr(pos + 1, this->URL.size());
    size_t pos2 = type.find("?");
    if (this->step >= 0 && (this->step < DONE || step == CHUNCKEDDONE ))
        return ;
    if (pos2 != std::string::npos)
        type.erase(pos2, type.size());
    if (type == "php" or type == "py" or type == "pl")
        return (void)(this->step = CGII);
    else if (type == "" or this->step < 0)
        return (void)(std::cout << "helllo" << std::endl,this->step = ERROR);
    else if (type == "png" or type == "jpeg" or type == "jpg")
        return (void)(this->step = BINARYFILE);
    else
        return (void)(this->step = NORMFILE);
}

void Req::check(){
    if (METHOD == "GET" or METHOD == "DELETE")
        this->step = DONE;
    else if (HEADERS.find("Content-Length") != HEADERS.end())
        this->step = 2;
    else if (HEADERS.find("Transfer-Encoding") != HEADERS.end() 
        && HEADERS.find("Transfer-Encoding")->second == "chunked")
        this->step = CHUNCKED;
}

int Req::parseHeaders(std::string&hd){
    std::string key;
    std::string value;
    std::string::iterator i = hd.begin();
    std::string::iterator j = hd.begin();

    if (hd == "\r")
            return check(),this->step;
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
    for (std::vector<std::string>::iterator it = this->_Config.a_meth.begin(); it != this->_Config.a_meth.end(); it++) {
	    if (method == *it)
		    return 0;
	}
    return -1;
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
        return (this->step = BADREQ);
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
        return this->step = -3;
    HTTPV = s;
    this->step = 1;
    return 0;
}

std::map<std::string, std::string> Req::getHEADERS(){
	return this->HEADERS;
}

void Req::clearData() {
    this->HEADERS.clear();
    this->METHOD = "";
    this->URL = "";
    this->HTTPV = "";
    this->Body = "";
    this->body.clear();
    this->step = 0;
    this->chunkSize = -1;
    this->var = 0;
}

int Req::getStep(){return step;};

int Req::getServerFd(){return this->ServerFd;}

int Req::getClientFd(){return this->clientFd;}

void Req::setStep(int s){this->step = s;}