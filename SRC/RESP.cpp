/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RESP.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrisse <megrisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 17:12:16 by megrisse          #+#    #+#             */
/*   Updated: 2023/06/03 03:25:27 by megrisse         ###   ########.fr       */
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
	errorsFiles[204] = "./ErrorFiles/204.html";
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
	this->Size_send = 0;
	this->status_line = "";
	this->response_header = "";
	this->fileData.clear();
	this->_headers.clear();
	this->Resp.clear();
	this->clearData();
	file_size = 0;
	code = 0;
	this->type = "";
	root = Conf.pRoot;
	index = Conf.index;
	autoInx = false;
	// root = "";
	// index = "";
}

Res::Res(struct config server, int serverfd, int clientfd) : Req(serverfd, clientfd, server){

    Conf = server;
	setMIME();
    initErrorFiles();
	code = 0;
	root = Conf.pRoot;
	index = Conf.index;
	autoInx = false;
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

void	Res::autoindex() {

	DIR* directory;
    struct dirent* entry;

    // Open the directory

	if (root[0] == '/')
		root = "." + root;
	std::string	html = "<!DOCTYPE html><html><head><title> autoindex </title></head><body>";
	html += "<h1> autoindex of " + root + "</h1>";
	std::cout << "ROOT == " << root << std::endl;
    directory = opendir(root.c_str());
    if (directory == NULL) {
        std::cerr << "Error opening directory." << std::endl;
    }
    // Read directory entries
    while ((entry = readdir(directory)) != NULL) {
		std::string file(entry->d_name);
		if (file[0] != '.')
			html += "<h3>" + file + "</h3>";
    }
	html += "</body></html>";
	for (size_t i = 0; i < html.length(); i++)
		fileData.push_back(html[i]);
	printvector(fileData, 1337);
	code = 200;
	type = "html";
	file_size = html.length();
    // Close the directory
    closedir(directory);
}

bool	Res::CheckIfLoction(std::string path) {

	std::vector<loca>::iterator it = Conf.vect.begin();

	for (; it != Conf.vect.end(); it++) {

		if (it->l_path == path)
			return std::cout << "DAZ MN HNAAAA +" << std::endl, root = path, this->index = it->index, true;
	}
	return false;
}

bool	Res::GetIfAutoIndex(std::string path) {

	std::vector<loca>::iterator it = Conf.vect.begin();

	for (; it != Conf.vect.end(); it++) {

		std::cout << "HH = " << it->autoIndex << std::endl;
		if (it->l_path == path and it->autoIndex == "on")
			return std::cout << "DAZ MN HNAAAA +++ "<< std::endl, true;
	}
	return false;
}

void	Res::getifQuerry(std::string &url) {

	if ((getURL() == "/" && !index.empty()))
		filePath = index;
	else if (CheckIfLoction(url))
		filePath = index;
	else if (index.empty() && Conf.autoIndex == "on")
		autoInx = true;
	else
		filePath = getFilePath(url);
	if (index.empty() && GetIfAutoIndex(url))
		autoInx = true;
	std::cout << "PATH = " << filePath << std::endl;
	size_t	pos = url.find("?");
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
	filePath = root + filePath;
	if (filePath[0] != '.')
		filePath = "." + filePath;
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
	else if (autoInx == true)
		autoindex();
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
	if (!checkCgipath(filePath) or type == "php" or type == "py" or type == "pl") {

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
	std::cout << "auto = " << index << std::endl;
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
	binary = false;
	std::string	boundry = getBoundry();
	body.erase(0, boundry.size() + 2);
	std::string	boundry2 = boundry + "--";
	pos1 = body.find(boundry2);
	if (pos1 != std::string::npos)
		body.erase(pos1, boundry2.size());
	else
		std::cout << "Boundary Not Found !" << std::endl, binary = true;
	pos1 = 0;
	pos1 = body.find("filename=", pos1);
	if (pos1 != std::string::npos)
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
	start = body.find_first_not_of("\n\r\t ");
	end = body.find_last_not_of("\n\r\t ");
	if (start == std::string::npos || end == std::string::npos)
		code = 400;
	else
		upld_body = body.substr(start, end - start + 1);
	if (binary == true)
		upld_body = getBody();
	upld_file_name = filename;
}

void	Res::getpathtoUp() {

	path_to_upld = getURL();
	if (getURL().find("/") != std::string::npos)
		path_to_upld.erase(std::remove(path_to_upld.begin(), path_to_upld.end(), '/'), path_to_upld.end());
}

void	Res::CreateFile() {

	std::ofstream file;
	std::string	file_name;
	Generatename();
	if (upld_file_name == "")
		upld_file_name = "upload." + type;
	file_name = path_to_upld + "/";
	file_name += upld_file_name;
	file.open(file_name, std::ios::binary);
	if (!file.is_open())
		code = 500;
	else {

		for (size_t i = 0; i < upld_body.size(); i++)
			file << upld_body[i];
		file.close();
		code = 201;
	}
	readErrorsfiles(errorsFiles[code]);
}

void	Res::CreatepairsFiles() {

	std::string file;
	std::ofstream f;
	std::map<std::string, std::string>::iterator it = pairs.begin();

	while (it != pairs.end()) {

		file = path_to_upld;
		file += "/" + it->first + ".txt";
		f.open(file.c_str());
		f << it->second;
		f.close();
		it++;
	}
}

void	Res::getPairs() {

	std::string	line = getBody();
	size_t		start = 0;
	size_t		end = 0;

	while (start != std::string::npos) {

		end = line.find('&', start);
		std::string	pair = line.substr(start, end - start);
		size_t p = pair.find('=');
		if (p != std::string::npos) {

			std::string key = pair.substr(0, p);
			std::string value = pair.substr(p + 1);
			if (value.find('\n') != std::string::npos)
				value.erase(std::remove(value.begin(), value.end(), '\n'), value.end());
			pairs[key] = value;
		}
		if (end == std::string::npos)
			break ;
		start = end + 1;
	}
	type = "txt";
}

void	Res::Generatename() {

	std::string	tt = getHEADERS().find("Content-Type")->second;
	iter_map it = mimeTypes.begin();
	for (; it != mimeTypes.end(); it++) {

		if (it->second == tt){
			
			type = it->first;
			break;
		}
	}
	if (tt == "")
		type = "txt";
}

void	Res::Handl_encoded() {

	getPairs();
	CreatepairsFiles();
	code = 201;
	readErrorsfiles(errorsFiles[code]);
}

void	Res::POST() {

	std::vector<std::string> cgiBuff;
	std::string	response_body = "";
	std::string	response = "";
	std::string tt = getHEADERS().find("Content-Type")->second;
	size_t pos = 0;
	pos = tt.find_last_of('/', tt.length());
	tt = tt.substr(pos + 1, tt.length() - pos);
	getpathtoUp();
	getifQuerry(getURL());
	if (!checkCgipath(filePath) or type == "php" or type == "pl") {

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
	else if (tt == "x-www-form-urlencoded")
		Handl_encoded();
	else if (getStep() == CHUNCKEDDONE && tt.find("boundary=") == std::string::npos) {

		upld_body = getBody();
		CreateFile();
	}
	else {

		beginInPOST();
		struct stat st;
		if (stat(path_to_upld.c_str(), &st) != 0)
			mkdir(path_to_upld.c_str(), 0777);
		type = tt;
		CreateFile();
	}
	getHeadersRes();
	mergeResponse();
}

void	Res::DELETE() {
	size_t start = getURL().find_first_of('/');
	
	filePath = getURL().substr(start + 1, getURL().size() - start);
	if (checkpath(filePath)) {

		if (access(filePath.c_str(), R_OK) != 0)
			code = 403;
		if (remove(filePath.c_str()) == 0)
			code = 204;
	}
	else
		code = 404;
	readErrorsfiles(errorsFiles[code]);
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
	else if (this->getMETHOD() == "DELETE")
		DELETE();
}