/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RESP.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrisse <megrisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 17:12:16 by megrisse          #+#    #+#             */
/*   Updated: 2023/05/25 17:08:07 by megrisse         ###   ########.fr       */
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
	std::cout << std::endl;
	std::cout << "---------------------------------------------------------\n";
}

void	Res::resetvalues() {

	this->Querry = "";
	this->filePath = "";
	// this->code = 200;
	this->Size_send = 0;
	this->status_line = "";
	this->fileData.clear();
	this->_headers.clear();
	this->clearData();
	file_size = 0;
	this->type = "";
}

Res::Res(struct config server, int serverfd, int clientfd) : Req(serverfd, clientfd, server){

    server = Conf;
	setMIME();
    
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
	std::cout << "new file path = " << _filePath << std::endl;

	return (_filePath);
	
}

void	Res::getifQuerry(std::string &url) {

	size_t	pos = url.find("?");

	filePath = getFilePath(url);
	if (pos != std::string::npos) 
		Querry = url.substr(pos + 1, url.length());
	pos = url.rfind(".");

	type = url.substr(pos + 1, url.size() - pos);
	std::cout << "Querry "<< Querry <<std:: endl;
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

void	Res::getHeadersRes() {

	std::string	headers;
	std::stringstream	ss;
	std::stringstream	sss;

	ss << code;
	getDate();
	status_line = "HTTP/1.1 " + ss.str() + " " + getStatusMsg(code) + CRLF;
	sss << file_size;
	headers = status_line + "Content-Type: " + mimeTypes[type] + CRLF;
	headers += "Content-length: " + sss.str() + CRLF;
	headers += "Date: " + Date + CRLF;
	for (size_t i = 0; i < headers.length(); i++)
		_headers.push_back(headers[i]);
}

void	Res::readContent() {

	std::ifstream	file;

	std::cout << "||" << Conf.pRoot << "||\n";
	filePath = "./www/" + filePath;
	std::cout << "||" << filePath << "||\n";
	if (checkpath(filePath)) {

		file.open(filePath, std::ios::in);
		if (!file.is_open()) {

			code = 403;
			readErrorsfiles(errorsFiles[code]);
		}
		file.seekg(0, std::ios::end);
    	std::streamsize fileSize = file.tellg();
		file_size = fileSize;
    	file.seekg(0, std::ios::beg);
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
	std::string	response_header = "";
	type = filePath.substr(filePath.rfind(".") + 1 , filePath.size() - filePath.rfind("."));
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
	getHeadersRes();
}

void	Res::buildNormalResponse() {

	getifQuerry(getURL());
	readContent();
}

void	Res::buildErrorResponse() {

	
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
    // testConnection(kevent(kq, &ev, 1, NULL, 0, NULL), "FAILDE ADD THE FD TO KEVENT");
}

void	Res::mergeResponse() {

	_headers.insert(_headers.end(), fileData.begin(), fileData.end());
	setSizesend(_headers.size());
}

// std::vector<char>	Res::makeResponse() {
	
// 	if (getSizeSend() >= _headers.size())
// }

// void	Res::sendResponse(int kq) {

// 	printvector(_headers, 5555);
// 	if (send(clientFd, _headers.data(), _headers.size(), 0) < 0)
// 		code = 500;
// 	else
// 		setSizesend(_headers.size());
// 	if (getSizeSend() >= _headers.size()) {

// 		keventUP(kq, clientFd, EVFILT_WRITE, EV_DISABLE);
// 		keventUP(kq, clientFd, EVFILT_READ, EV_CLEAR | EV_ENABLE | EV_ADD);
// 		// resetvalues();
// 	}
// 	else
// 		keventUP(kq, clientFd, EVFILT_WRITE, EV_ENABLE);
// 	updateTime();
// }

void	Res::GET() {

	switch (getStep()) {

		case ERROR :
			buildErrorResponse();
			break;
		case NORMFILE :
			std::cout << "ZEBIIII 4 \n", buildNormalResponse();
			break;
		case BINARYFILE :
			std::cout << "ZEBIIII 5 \n", buildNormalResponse();
			break;
		case CGII :
			buildCGIResponse();
			break ;
	}
	mergeResponse();
	printvector(_headers, 88);
}

void	Res::buildResponse() {
	
	if (this->getMETHOD() == "GET")
		GET();
		
}