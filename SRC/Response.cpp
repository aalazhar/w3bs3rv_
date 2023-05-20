/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hameur <hameur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 21:20:47 by megrisse          #+#    #+#             */
/*   Updated: 2023/05/20 23:15:01 by hameur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/Response.hpp"

void Response::setLocation(struct config &server){

	std::cout << "HAAAAAA \n";
	this->Locations.a_meth = server.a_meth;
	std::cout << "HAAAAAA1 \n";
	this->Locations.allowed_m = server.allowed_m;
	std::cout << "HAAAAAA2 \n";
	this->Locations.autoIndex = server.autoIndex;
	std::cout << "HAAAAAA3 \n";
	for (std::vector<std::string>::iterator it = server.err_p.begin(); it != server.err_p.end();it++){
		std::cout << "zeebi : " << *it << std::endl;
	}
	this->Locations.err_p = server.err_p;
	std::cout << "HAAAAAA4 \n";
	this->Locations.error_page = server.error_page;
	std::cout << "HAAAAAA5 \n";
	this->Locations.index = server.index;
	std::cout << "HAAAAAA6 \n";
	this->Locations.listen = server.listen;
	std::cout << "HAAAAAA7 \n";
	this->Locations.lsten = server.lsten;
	std::cout << "HAAAAAA8 \n";
	this->Locations.port = server.port;
	std::cout << "HAAAAAA \n";
	this->Locations.pRoot = server.pRoot;
	std::cout << "HAAAAAA \n";
	this->Locations.s_name = server.s_name;
	std::cout << "HAAAAAA \n";
	this->Locations.s_names = server.s_names;
	std::cout << "HAAAAAA \n";
	this->Locations.upload = server.upload;
	std::cout << "HAAAAAA \n";
	this->Locations.uploads = server.uploads;
	std::cout << "HAAAAAA \n";
	this->Locations.vect = server.vect;
	std::cout << "HAAAAAA \n";
}


Response::Response(struct config &server, int serverfd, int clientfd) :Req(serverfd, clientfd){
	std::cout << "heloo1\n";
	// this->Locations = server;
	this->setLocation(server);
	std::cout << "heloo2\n";
	code = 200;
	std::cout << "heloo3\n";
	AllowedM = server.allowed_m;
	std::cout << "heloo4\n";
	if (Locations.pRoot != "")	
		root = Locations.pRoot;
	std::cout << "heloo5\n";
	if (server.autoIndex == "on")
		autoInx = true;
	else
		autoInx = false;
	std::cout << "heloo\n";
	this->r = 0;
}

Response::~Response() {

	
}

void	Response::initResponse() {

	size_t pos = getMETHOD().find(AllowedM);

	if (pos == std::string::npos)
		code = 405;
	
}

// Response    &Response::operator=(Req &obj) {

// 	// if (this != &obj)
// 	// 	this->response_headers = obj.getHEADERS();
// 	return (*this);
// }

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

int	Response::readcontent() {

	std::ifstream	file;
	std::string		path;	
	std::stringstream	resp;

	response = "";
	path = root + this->getURL();
	std::cout << "test : " << path << std::endl;
	if (checkpath(path)) {

		file.open(path.c_str(), std::ifstream::in);
		if (!file.is_open()) {
			
			response_body = readErrorsfiles(errorsFiles[403]);
			return 403;
		}
		resp << file.rdbuf();
		response_body = resp.str();
		file.close();
	}
	else 
		return (response_body = readErrorsfiles(errorsFiles[404]), 404);
	return 200;
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

	std::string	type;
	
	type = URL.substr(URL.rfind(".") + 1, URL.size() - URL.rfind("."));
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
	status_line = "HTTP/1.1 " + ss.str() + " " + getStatusMsg(code) ;
	sss << response_body.size();
	if (type != "")
		headers += "Content-Type: " + getContentType() + CRLF;
	headers += "Content-length: " + sss.str() + CRLF;
	headers += "Date: " + Date + CRLF;
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

std::string	Response::readErrorsfiles(std::string path) {

	std::stringstream	respon;
	std::ofstream		file;
	Response::initErrorFiles();

	if (checkpath(path)) {
		
		file.open(path.c_str(), std::ifstream::in);
		if (file.is_open() == false)
			return "<!DOCTYPE html><html><title> 4444 Error: Error File Not Found </title><body><div><h1> 4444 Error File Not Found </h1><p> We're sorry, the page you requested could not be found.</p></div></body></html>";
		respon << file.rdbuf();
		file.close();
		type = "text/html";
		return (respon.str());
	}
	else
		return "<!DOCTYPE html><html><title> 4444 Error: Error File Not Found </title><body><div><h1> 4444 Error File Not Found </h1><p> We're sorry, the page you requested could not be found.</p></div></body></html>";
}


int 			Response::makeResponse() {
	// std::cout << "heeeeelllloo\n";
	Req *req = dynamic_cast<Req *>(this);
	int ret = 0;
	this-> r = 0;
	ret = GetMethod(*req);
	return 1;
}

std::string	vectostring(std::vector<std::string> vec) {

	// size_t size = vec.size();

	std::string ret = "";
	std::vector<std::string>::iterator it = vec.begin();
	while (it != vec.end()) {

		ret += *it;
		it++;
	}
	return (ret);
}

int	Response::GetMethod(Req &obj) {
	
	getifQuerry(obj.getURL());
	CGI	cgi("./CGI-bin/php-cgi", obj.getMETHOD(), type, filePath, obj.getBody(), Querry, obj.getBody().length());
	if (obj.getStep() == -1) {

		code = 405;
		response_body = readErrorsfiles(errorsFiles[code]);
	}
	else if (obj.getStep() == -3)
		code = 505;
	if (!checkCgipath(filePath)) {

		size_t	i = 0;
		size_t	size = response.size() - 2;

		_response = cgi.executeCGI();
		response = vectostring(_response);
		while (response.find("\r\n\r\n", i) != std::string::npos || response.find("\r\n\r\n", i) == i) {

			std::string	resp = response.substr(i, response.find("\r\n", i) - i);
			if (!resp.find("Status: "))
				code = std::atoi(resp.substr(8, 3).c_str());
			else if (!resp.find("Content-type: "))
				type = resp.substr(14, resp.size());
			i += resp.size() + 2;
		}
		while (response.find("\r\n", size) == size)
			size -= 2;

		response_body = response.substr(i, size - i);
	}
	else if (code == 200)
		code = readcontent();
	if (code == 404)
		response_body = readErrorsfiles(errorsFiles[code]);
	if (code == 500)
		response_body = readErrorsfiles(errorsFiles[code]);
	response_header = getResponseHeader();
	return code;
}
