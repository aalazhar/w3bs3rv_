/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RESP.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrisse <megrisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 17:12:16 by megrisse          #+#    #+#             */
/*   Updated: 2023/05/28 00:45:07 by megrisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/RESP.hpp"

void	printvector(std::vector<char> vec, int key) {


	std::vector<char>::iterator it = vec.begin();
	std::cout << "---------------------------------------------------------\n";
	std::cout << "VECTOR " << key << " :\n";
	for (; it != vec.end(); it++) {

		std::cout << *it;
	}
	std::cout << "---------------------------------------------------------\n";
}

void	Res::initErrorFiles() {

	errorsFiles[201] = "./ErrorFiles/201.html";
	errorsFiles[400] = "./ErrorFiles/400.html";
	errorsFiles[403] = "./ErrorFiles/403.html";
	errorsFiles[404] = "./ErrorFiles/404.html";
	errorsFiles[405] = "./ErrorFiles/405.html";
	errorsFiles[408] = "./ErrorFiles/408.html";
	errorsFiles[410]  = "./ErrorFiles/410.html";
	errorsFiles[413]  = "./ErrorFiles/413.html";
	errorsFiles[500]  = "./ErrorFiles/500.html";
}

void	Res::resetvalues() {

	this->Querry = "";
	this->filePath = "";
	// this->code = 200;
	this->Size_send = 0;
	this->status_line = "";
	this->response_header = "";
	this->fileData.clear();
	this->_headers.clear();
	this->Resp.clear();
	this->clearData();
	file_size = 0;
	this->type = "";
}

Res::Res(struct config server, int serverfd, int clientfd) : Req(serverfd, clientfd, server){

    Conf = server;
	setMIME();
    initErrorFiles();
	code = 0;
}

std::string getFilePath(std::string url){

	int pos = url.find('?');
	std::string quiry = url;
	std::string _filePath = url;
	int count = 0;
	if (pos == -1)
		pos = url.size();
	else if (pos >= 0)
		url[pos] = '\0';
	while (--pos){
		if (url[pos] == '/')
			break;
		count++;			
	}
	_filePath = url.substr(pos + 1, count);
	return (_filePath);
	
}

void	Res::getifQuerry(std::string &url) {

	size_t	pos = url.find("?");

	filePath = getFilePath(url);
	if (pos != std::string::npos) 
		Querry = url.substr(pos + 1, url.length());
	pos = url.rfind(".");

	type = url.substr(pos + 1, url.size() - pos);
}

int	checkpath(std::string &path) {

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

void	Res::initCodesMsgs() {

	CodesMsgs[100] = "Continue";
	CodesMsgs[200] = "OK";
	CodesMsgs[201] = "Created";
	CodesMsgs[204] = "No Content";
	CodesMsgs[400] = "Bad Request";
	CodesMsgs[403] = "Forbidden";
	CodesMsgs[404] = "Not Found";
	CodesMsgs[405] = "Method Not Allowed";
	CodesMsgs[408] = "Request Timeout";
	CodesMsgs[413] = "Payload Too Large";
	CodesMsgs[500] = "Internal Server Error";
}

void	Res::readErrorsfiles(std::string path) {

	std::ifstream		file;

	type = "html";
	if (checkpath(path)) {
		
		file.open(path.c_str(), std::ifstream::in);
		file.seekg(0, std::ios::end);
		std::streamsize	fileSize = file.tellg();
		file.seekg(0, std::ios::beg);
		std::vector<char>	data(fileSize);
		file.read(data.data(), fileSize);
		file_size = fileSize;
		fileData = data;
		file.close();
	}
}

void	Res::getDate() {

	std::time_t currentTime = std::time(NULL);
    std::string dateString1 = std::ctime(&currentTime);
	Date = std::string(dateString1);
}

std::string	Res::getStatusMsg(int code) {

	initCodesMsgs();
	if (CodesMsgs.find(code) != CodesMsgs.end())
		return CodesMsgs[code];
	else
		return "INVALID CODE !";
}

std::string	Res::getMimetype(std::string type) {

	return (mimeTypes[type]);
}

void	Res::getHeadersRes() {

	std::string	headers = "";
	std::stringstream	ss;
	std::stringstream	sss;

	ss << code;
	getDate();
	status_line = "HTTP/1.1 " + ss.str() + " " + getStatusMsg(code) + CRLF;
	sss << file_size;
	if (getStep() == CGII)
		headers = status_line + response_header;
	else
		headers = status_line + "Content-Type: " + getMimetype(type) + CRLF;
	headers += "Content-length: " + sss.str() + CRLF;
	headers += "Date: " + Date + CRLF;
	for (size_t i = 0; i < headers.size(); i++)
		_headers.push_back(headers[i]);
}

void	Res::readContent() {

	std::ifstream	file;

	std::cout << "HA root " << Conf.pRoot << std::endl;
	filePath = Conf.pRoot + filePath;//hna ra Proot khawhi fi config;
	std::cout << "HA Lfile kml " << filePath << std::endl;
	if (checkpath(filePath)) {

		file.open(filePath, std::ios::in);
		if (!file.is_open()) {

			code = 403;
			readErrorsfiles(errorsFiles[code]);
		}
		file.seekg(0, std::ios::end);
    	std::streamsize fileSize = file.tellg();
    	file.seekg(0, std::ios::beg);
		file_size = fileSize;
		std::vector<char>	Data(fileSize);
		file.read(Data.data(), fileSize);
		fileData = Data;
		file.close();
		code = 200;
	}
	else {

		code = 404;
		readErrorsfiles(errorsFiles[code]);
	}
	getHeadersRes();
}

int	Res::checkCgipath(std::string &path) {

	std::vector<loca>::iterator	it = Conf.vect.begin();
	while (it != Conf.vect.end()) {

		if (it->cgiPath == path)
			return (_Cgipath = it->cgiPath, 0);
		it++;
	}
	return 1;
}

std::string vectorToString(const std::vector<std::string>& vec) {
    std::ostringstream oss;
    for (std::vector<std::string>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
        oss << *it;
    }
    return oss.str();
}

void	Res::buildCGIResponse() {

	std::vector<std::string> cgiBuff;
	std::string	response_body = "";
	std::string	response = "";
	std::string	t = "";
	response_header = "";
	getifQuerry(getURL());

	t = filePath.substr(filePath.rfind(".") + 1 , filePath.size() - filePath.rfind("."));
	type = t;
	if (!checkCgipath(filePath) or type == "php" or type == "py") {

		CGI	cgi(filePath, getMETHOD(), type, "", getBody(), Querry, getBody().length());
		size_t	i = 0;
		size_t	size = response.size() - 2;
		cgiBuff = cgi.executeCGI();
		response = vectorToString(cgiBuff);
		while (response.find("\r\n\r\n", i) != std::string::npos || response.find("\r\n\r\n", i) == i) {

			std::string	resp = response.substr(i, response.find("\r\n", i) - i);
			response_header += resp;
			response_header += CRLF;
			if (!resp.find("Status: "))
				code = std::atoi(resp.substr(8, 3).c_str());
			else if (!resp.find("Content-type: "))
				this->type = resp.substr(14, resp.size());
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
		code = 200;
	}
	else {
		
		code = 404;
		readErrorsfiles(errorsFiles[code]);
	}
	getHeadersRes();
	mergeResponse();
}

void	Res::buildNormalResponse() {

	getifQuerry(getURL());
	readContent();
	mergeResponse();
}

void	Res::buildErrorResponse() {


	if (getStep() == TIMEOUT)
		code = 408;
	else
		code = 400;
	readErrorsfiles(errorsFiles[code]);
	getHeadersRes();
	mergeResponse();
}

void Res::keventUP(int kq, int fd, int filter, int flag){
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
}

void	Res::mergeResponse() {

	for (size_t i = 0; i < _headers.size() + fileData.size(); i++) {

		if (i < _headers.size())
			Resp.push_back(_headers[i]);
		else {

			Resp.push_back(fileData[i - _headers.size()]);
		} 
	}
}

void	Res::GET() {

	switch (getStep()) {

		case NORMFILE :
			buildNormalResponse();
			break;
		case BINARYFILE :
			buildNormalResponse();
			break;
		case CGII :
			buildCGIResponse();
			break ;
	}
}

std::string	Res::getBoundry() {

	std::string	headers = getHEADERS()["Content-Type"];
	size_t	pos =  0;
	pos = headers.find("boundary=", pos);
	std::string boundry = "--" + headers.erase(0, pos + 9);
	// std::cout << "POS = " << pos << std::endl;
	// std::string boundry = headers.substr(pos + 9, headers.length());
	std::cout << "BOOUND : " << boundry << std::endl;
	return boundry;
}

void	Res::getUpFname(std::string body) {

	std::string from = "filename=\"";
	std::string sh;
	if (body.find(from) != std::string::npos) {

		size_t pos = 0;
		size_t pos1 = 0;
		pos = body.find(from, pos);
		pos1 = body.find("Content-Type:", pos1);
		sh = body.substr(pos + from.size(), pos1);
		pos = sh.find("\"");
		upld_file_name = sh.substr(0, pos);
		std::cout << "file = " << upld_file_name << "|" << std::endl;
	}
}

void	Res::beginInPOST() {

	std::string	body = getBody();
	std::string filename = "";
	size_t	pos1 = 0;
	std::string	boundry =getBoundry();
	body.erase(0, boundry.size() + 2);
	std::string	boundry2 = boundry + "--";
	pos1 = body.find(boundry2);
	if (pos1 != std::string::npos)
		body.erase(pos1, boundry2.size());
	else
		std::cout << "Boundary Not Found !" << std::endl;
	pos1 = 0;
	pos1 = body.find("filename=", pos1);
	for (int i = pos1 + 10; body[i] != '"'; i++)
		filename.push_back(body[i]);
	size_t start = 0;
	size_t end = 0;
	start = body.find("Content-Disposition", start);
	end = body.find("\n", start);
	if (start != std::string::npos && end != std::string::npos)
		body.erase(start, end);
	start = 0;
	end = 0;
	start = body.find("Content-Type", start);
	end = body.find("\n", start);
	if (start != std::string::npos && end != std::string::npos)
		body.erase(start, end);
	upld_body = body;
	upld_file_name = filename;
}

void	Res::getpathtoUp() {

	path_to_upld = getURL();
	if (getURL().find("/") != std::string::npos)
		path_to_upld.erase(std::remove(path_to_upld.begin(), path_to_upld.end(), '/'), path_to_upld.end());
	std:: cout << "PATH TO UP = " << path_to_upld << "|" << std::endl;
}

void	Res::CreateFile() {

	std::ofstream file;
	std::string	file_name;
	file_name = path_to_upld + "/";
	file_name += upld_file_name;
	file.open(file_name);
	for (size_t i = 0; i < upld_body.size(); i++)
		file << upld_body[i];
	file.close();
	code = 201;
	readErrorsfiles(errorsFiles[code]);
}

void	Res::POST() {
	
	beginInPOST();
	if (getBody().empty()) {

		code = 400;
		readErrorsfiles(errorsFiles[code]);
	}
	getpathtoUp();
	struct stat st;
	if (stat(path_to_upld.c_str(), &st) != 0) {

		std::cout << "PPPP" << std::endl;
		mkdir(path_to_upld.c_str(), 0777);
	}
	CreateFile();
	getHeadersRes();
	mergeResponse();
}

void	Res::buildResponse() {

	if (this->getStep() == ERROR or  this->getStep() == TIMEOUT)
		buildErrorResponse();
	else if (this->getMETHOD() == "POST")
		POST();
	else if (this->getMETHOD() == "GET")
		GET();
}