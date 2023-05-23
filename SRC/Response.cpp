/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrisse <megrisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 21:20:47 by megrisse          #+#    #+#             */
/*   Updated: 2023/05/23 17:06:14 by megrisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/Response.hpp"

void printVec(std::vector<std::string> vec){
	for(size_t i = 0; i < vec.size(); i++){
		std::cout << vec[i] << std::endl;
	}
}

void Response::setLocation(struct config &server){
	this->Locations.a_meth = server.a_meth;
	this->Locations.allowed_m = server.allowed_m;
	this->Locations.autoIndex = server.autoIndex;
	this->Locations.err_p = server.err_p;
	this->Locations.error_page = server.error_page;
	this->Locations.index = server.index;
	this->Locations.listen = server.listen;
	this->Locations.lsten = server.lsten;
	this->Locations.port = server.port;	this->Locations.pRoot = server.pRoot;	this->Locations.s_name = server.s_name;	printVec(server.s_names);
	this->Locations.s_names = server.s_names;	this->Locations.upload = server.upload;	printVec(server.uploads);
	this->Locations.uploads = server.uploads;	this->Locations.vect = server.vect;
}


Response::Response(struct config &server, int serverfd, int clientfd) :Req(serverfd, clientfd, server){
	this->Locations = server;	
	// this->setLocation(server);
	code = 200;
	if (Locations.pRoot != "")	
		root = Locations.pRoot;
	if (server.autoIndex == "on")
		autoInx = true;
	else
		autoInx = false;
	this->r = 0;
	initErrorFiles();
}

void	Response::resetvalues() {

	this->Querry = "";
	this->filePath = "";
	this->code = 200;
	this->status_line = "";
	this->response_header = "";
	this->fileData.clear();
	this->_headers.clear();
	file_size = 0;
	headers_size = 0;
	this->type = "";
	this->clear();
}

Response::~Response() {

	// std::cout << "KHREEEEEEEEEEEEJ \n";
}

int	Response::getifQuerry(std::string &url) {

	size_t	pos = url.find("?");

	if (pos != std::string::npos) {

		filePath = url.substr(0, pos);
		Querry = url.substr(pos + 1, url.length());
		return 0;
	}
	filePath = url;
	Querry = "";
	return 1;
}

int	Response::checkpath(std::string &path) {

	struct stat file_st;

	if (!stat(path.c_str(), &file_st)) {

		if (S_ISREG(file_st.st_mode))
			return 1;
		else if (S_ISDIR(file_st.st_mode))
			return 0;
		else
			return 0;
	}
	return 0;
}

void	Response::Readimage(std::string path) {

	std::ifstream file(path, std::ios::binary);
	if (!file.is_open()) {

		code = 403;
		readErrorsfiles(errorsFiles[code]);
    }
	file.seekg(0, std::ios::end);
    std::streamsize fileSize = file.tellg();
	file_size = fileSize;
    file.seekg(0, std::ios::beg);
	std::vector<char>	Data(fileSize + 2);
	file.read(Data.data(), fileSize);
	fileData = Data;
	file.close();
}

void	Response::printvector(std::vector<char> vec, int key) {


	std::vector<char>::iterator it = vec.begin();
	std::cout << "---------------------------------------------------------\n";
	std::cout << "VECTOR " << key << " :\n";
	for (; it != vec.end(); it++) {

		std::cout << *it;
	}
	std::cout << std::endl;
	std::cout << "---------------------------------------------------------\n";
}

void	Response::ReadFile(std::string path) {

	std::ifstream		file;

	file.open(path.c_str(), std::ios::in);
	if (!file.is_open()) {

		code = 403;
		readErrorsfiles(errorsFiles[code]);
	}
	file.seekg(0, std::ios::end);
    std::streamsize fileSize = file.tellg();
	file_size = fileSize;
    file.seekg(0, std::ios::beg);
	std::vector<char>	Data(fileSize + 2);
	file.read(Data.data(), fileSize);
	fileData = Data;
	file.close();
}

int	Response::readcontent() {

	std::ifstream		file;
	std::string			path;
	std::stringstream	resp;

	response = "";
	path = root + this->getURL();
	if (checkpath(path)) {

		type = URL.substr(URL.rfind(".") + 1, URL.size() - URL.rfind("."));
		if (type == "png" or type == "jpeg" or type == "jpg")
			Readimage(path);
		else
			ReadFile(path);
	}
	else {

		code = 404;
		readErrorsfiles(errorsFiles[code]);
	}
	return code;
}

int	Response::checkCgipath(std::string &path) {

	std::vector<loca>::iterator	it = Locations.vect.begin();
	while (it != Locations.vect.end()) {

		if (it->cgiPath == path)
			return (_Cgipath = it->cgiPath, 0);
		it++;
	}
	return 1;
}

void	Response::initErrorMsgs() {

	errors[100] = "Continue";
	errors[200] = "OK";
	errors[201] = "Created";
	errors[204] = "No Content";
	errors[400] = "Bad Request";
	errors[403] = "Forbidden";
	errors[404] = "Not Found";
	errors[405] = "Method Not Allowed";
	errors[413] = "Payload Too Large";
	errors[500] = "Internal Server Error";
}

std::string	Response::getStatusMsg(int code) {

	if (errors.find(code) != errors.end())
		return errors[code];
	else
		return "INVALID CODE !";
}

std::string	Response::getContentType() {

	if (type == "html")
		return "text/html";
	else if (type == "css")
		return "text/css";
	else if (type == "js")
		return "text/javascript";
	else if (type == "jpeg" || type == "jpg")
		return "image/jpeg";
	else if (type == "png")
		return "image/png";
	else
		return "text/plain";
}

void	Response::getDate() {

	std::time_t currentTime = std::time(NULL);
    std::string dateString1 = std::ctime(&currentTime);
	Date = std::string(dateString1);
}

std::string	Response::getResponseHeader() {

	std::string	headers("");
	std::stringstream	ss;
	std::stringstream	sss;


	initErrorMsgs();
	ss << code;
	getDate();
	status_line = "HTTP/1.1 " + ss.str() + " " + getStatusMsg(code) + CRLF;
	sss << getFileSize();
	headers = status_line + response_header;
	if (type != "" and type.length() <= 4)
		headers += "Content-Type: " + getContentType() + CRLF;
	headers += "Content-length: " + sss.str() + CRLF;
	headers += "Date: " + Date + CRLF;
	headers_size = headers.length();
	return (headers);
}

void	Response::initErrorFiles() {
	//add the correct lien to the files
	errorsFiles[400] = "./ErrorFiles/400.html";
	errorsFiles[403] = "./ErrorFiles/403.html";
	errorsFiles[404] = "./ErrorFiles/404.html";
	errorsFiles[405] = "./ErrorFiles/405.html";
	errorsFiles[410]  = "./ErrorFiles/410.html";
	errorsFiles[413]  = "./ErrorFiles/413.html";
	errorsFiles[500]  = "./ErrorFiles/500.html";
}

void	Response::readErrorsfiles(std::string path) {

	// std::stringstream	respon;
	std::ifstream		file;

	type = "html";
	if (checkpath(path)) {
		
		file.open(path.c_str(), std::ifstream::in);
		// if (!file.is_open())
		// 	fileData.push_back("");// return ("<!DOCTYPE html><html><title> 4444 Error: Error File Not Found </title><body><div><h1> 4444 Error File Not Found </h1><p> We're sorry, the page you requested could not be found.</p></div></body></html>");
		file.seekg(0, std::ios::end);
		std::streamsize	fileSize = file.tellg();
		file.seekg(0, std::ios::beg);
		std::vector<char>	data(fileSize);
		file.read(data.data(), fileSize);
		fileData = data;
		file.close();
	}
	// else
		// return "<!DOCTYPE html><html><title> 4444 Error: Error File Not Found </title><body><div><h1> 4444 Error File Not Found </h1><p> We're sorry, the page you requested could not be found.</p></div></body></html>";
}


void	Response::buildResponse(Req &obj, int kq) {

	if (obj.getStep() == -1) {

		code = 405;
		readErrorsfiles(errorsFiles[code]);
	}
	else if (obj.getStep() == -3) {

		code = 505;
		readErrorsfiles(errorsFiles[code]);
	}
	else 
		makeResponse(obj, kq);
		
}

void 			Response::makeResponse(Req &req, int kq) {

	int ret = 0;
	this->r = 0;

	if (req.getMETHOD() == "GET")
		ret = GetMethod(req);
	sendResponse(req.getClientFd(), kq);
	std:: cout << "SALA SEND \n";
}

void	Response::sendHeaders(int fd, std::string headers) {

	std::cout << "HEADERS : " << headers << "------------------------" << " SIZEh = " << headers.length() << std::endl;;
	if (send(fd, headers.c_str(), headers.length(), 0) < 0)
		code = 500;
	this->setR(getHeadersSize());
	std::cout << "headers R === " << getR() << std::endl;
}

void	Response::sendBody(int fd, char *data, size_t size) {

	std::cout << "BODY : " << data << "----------------------" << " SIZEb = " << strlen(data) << std::endl;
	std::cout << "Body R 0=== " << getR() << std::endl;
	if (send(fd, data, size - 1, 0) < 0)
		code = 500;
	std::cout << "FILE SIZE = " << file_size <<std::endl;
	this->setR(this->getR() + getFileSize());
	std::cout << "Body R 1=== " << getR() << std::endl;
}

void Response::keventUP(int kq, int fd, int filter, int flag){
    struct kevent ev;
    struct timespec timeout;
     if (clock_gettime(CLOCK_REALTIME, &timeout) == -1) {
        perror("clock_gettime");
        return ;
    }
    EV_SET(&ev, fd, filter, flag, 0, 0, &timeout);
	if (kevent(kq, &ev, 1, NULL, 0, NULL) < 0) {

		std::cerr << "KEVENT FAILDE !!" << std::endl;
		exit(EXIT_FAILURE);
	}
    // testConnection(kevent(kq, &ev, 1, NULL, 0, NULL), "FAILDE ADD THE FD TO KEVENT");
}

void	Response::sendResponse(int Fd, int kq) {

	_headers.insert(_headers.end(), fileData.begin(), fileData.end());
	std::cout << "SIZE = " << _headers.size() << std::endl;
	std::cout << "KOLSHI = " << fileData.size() + response_header.length() << std::endl;
	if (send(Fd, _headers.data(), _headers.size(), 0) < 0)
		code = 500;
	this->setR(_headers.size());
	if (getR() >= getHeadersSize() + getFileSize()) {

		keventUP(kq, Fd, EVFILT_WRITE, EV_DISABLE);
		keventUP(kq, Fd, EVFILT_READ, EV_CLEAR | EV_ENABLE | EV_ADD);
		this->clear();
		resetvalues();
	}
	else
		keventUP(kq, Fd, EVFILT_WRITE, EV_ENABLE);
	this->updateTime();
}

std::string	vectostring(std::vector<std::string> vec) {

	std::string ret = "";
	std::vector<std::string>::iterator it = vec.begin();
	while (it != vec.end()) {

		ret += *it;
		it++;
	}
	return (ret);
}

std::vector<char> Response::convertTocharVec(std::vector<std::string> cgiBuff){
	std::vector<std::string>::iterator it = cgiBuff.begin();
	std::vector<char> _new;
	size_t i = 0;
	while (it != cgiBuff.end()){
		i = 0;
		while (i < (*it).size()){
			_new.push_back((*it)[i]);
			i++;
		}
		it++;
	}
	return (_new);
}

std::string vectorToString(const std::vector<std::string>& vec) {
    std::ostringstream oss;
    for (std::vector<std::string>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
        oss << *it;
    }
    return oss.str();
}

int	Response::GetMethod(Req &obj) {
	std::vector<std::string> cgiBuff;
	getifQuerry(obj.getURL());
	CGI	cgi("/UTILS/fileCGI.php", obj.getMETHOD(), type, filePath, obj.getBody(), Querry, obj.getBody().length());
	std::string tt = "";
	size_t j = 0;
	while (j < filePath.size()){
		if (filePath[j] == '.')
			break;
		j++;
	}
	
	tt = filePath.erase(0, j + 1);
	// tt = filePath.substr(filePath.rfind(".", pos) + 1, filePath.length());
	if (!checkCgipath(filePath) or tt == "php" or tt == "py") {

		size_t	i = 0;
		size_t	size = response.size() - 2;
		cgiBuff = cgi.executeCGI();
		this->fileData = convertTocharVec(cgiBuff);
		response = vectorToString(cgiBuff);
		while (response.find("\r\n\r\n", i) != std::string::npos || response.find("\r\n\r\n", i) == i) {

			std::string	resp = response.substr(i, response.find("\r\n", i) - i);
			response_header += resp;
			response_header += CRLF;
			if (!resp.find("Status: "))
				code = std::atoi(resp.substr(8, 3).c_str());
			else if (!resp.find("Content-type: "))
				type = resp.substr(14, resp.size());
			i += resp.size() + 2;
		}
		while (response.find("\r\n", size) == size)
			size -= 2;
		response_body = response.substr(i, size - i);
		if (fileData.size() > 0)
			fileData.clear();
		for (size_t i = 0; i < response_body.length(); i++)
			fileData.push_back(response_body[i]);
		file_size = fileData.size();
	}
	else if (code == 200)
		code = readcontent();
	if (code == 500)
		readErrorsfiles(errorsFiles[code]);
	response_header = getResponseHeader();
	if (_headers.size() > 1)
		_headers.clear();
	for (size_t i = 0; i < response_header.length(); i++)
		_headers.push_back(response_header[i]);
	return code;
}
