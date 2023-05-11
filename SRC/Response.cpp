/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrisse <megrisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 21:20:47 by megrisse          #+#    #+#             */
/*   Updated: 2023/05/11 17:12:06 by megrisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/Response.hpp"

Response::~Response() {

	
}

Response    &Response::operator=(Req &obj) {

	if (this != &obj)
		this->response_headers = obj.getHEADERS();
	return (*this);
}

std::string	Response::getCgiPath() {

	std::string	path;

	path = this->_config.;
}

void	Response::initEnvirement() {

	this->_env["REQUEST_METHOD"] = this->getMETHOD();
	if (HEADERS.find("Content-type") != HEADERS.end())
		this->_env["CONTENT_TYPE"] = this->HEADERS["Content-type"];
	if (HEADERS.find("Content-length") != HEADERS.end())
		this->_env["CONTENT_LENGTH"] = this->HEADERS["Content-length"];
	if (HEADERS.find("Host") != HEADERS.end())
		this->_env["SERVER_NAME"] = HEADERS["Host"];
	this->_env["QUERY_STRING"];
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["SERVER_SOFTWARE"];
	this->_env["SCRIPT_FILENAME"] = this->URL;
	this->_env["REDIRECT_STATUS"] = "200";
	this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
}

int	Response::readcontent() {

	std::ifstream	file;
	std::stringstream	resp;

	response = "";

	if (checkpath(this->getURL())) {

		file.open(this->getURL().c_str(), std::ifstream::in);
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
}

int	Response::checkpath(std::string &path) {

	struct stat file_st;

	if (stat(path.c_str(), &file_st) == 0) {
		
		if (S_ISREG(file_st.st_mode))
			return 1;
		if (S_ISDIR(file_st.st_mode))
			return 0;
		else
			return 0;
	}
	return 0;
}

std::string	Response::executeCgi(std::string file_name) {
	
	pid_t	pid;
	int		In;
	int		Out;
	int		rd;
	std::string	newbody;

	In = dup(STDIN_FILENO);
	Out = dup(STDOUT_FILENO);

	FILE	*fdIn = tmpfile();
	FILE	*fdOut = tmpfile();

	long	fileIn = fileno(fdIn);
	long	fileOut = fileno(fdOut);
	
	write(fileIn, this->Body.c_str(), this->Body.length());
	lseek(fileIn, 0, SEEK_SET);
	
	pid = fork();

	if (pid == -1) {

		std::cerr << "Fork !! \n";
		return ("Status: 500\r\n\r\n");
	}
	else if (!pid) {

		dup2(fileIn, STDIN_FILENO);
		dup2(fileOut, STDOUT_FILENO);
		char **env;//_env vector to char **
		execve(file_name.c_str(), NULL, env);
		std::cerr << "Execve !!" << std::endl;
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
	}
	else {

		char	buffer[1024] = {0};

		waitpid(-1, NULL, 0);
		lseek(fileOut, 0, SEEK_SET);
		
		rd = 1;
		while (rd > 0) {

			memset(buffer, 0, 1024);
			rd = read(fileOut, buffer, 1024);
			newbody += buffer;
		}
	}
	dup2(In, STDIN_FILENO);
	dup2(Out, STDOUT_FILENO);
	fclose(fdIn);
	fclose(fdOut);
	close(fileIn);
	close(fileOut);
	close(In);
	close(Out);
	//
	return newbody;
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

void	Response::initErrorFiles() {

	errorsFiles[400] = "/Users/megrisse/Desktop/Webserver_/ErrorFiles/400.html";
	errorsFiles[403] = "/Users/megrisse/Desktop/Webserver_/ErrorFiles/403.html";
	errorsFiles[404] = "/Users/megrisse/Desktop/Webserver_/ErrorFiles/404.html";
	errorsFiles[405] = "/Users/megrisse/Desktop/Webserver_/ErrorFiles/405.html";
	errorsFiles[410]  = "/Users/megrisse/Desktop/Webserver_/ErrorFiles/410.html";
	errorsFiles[413]  = "/Users/megrisse/Desktop/Webserver_/ErrorFiles/413.html";
	errorsFiles[500]  = "/Users/megrisse/Desktop/Webserver_/ErrorFiles/500.html";
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

int	Response::GetMethod(Req obj) {
	
	std::string	IfCgi = obj.getURL().substr(obj.getURL().find(".", obj.getURL().length()));
	code = 200;

	if (IfCgi == ".cgi" || IfCgi == ".php" || IfCgi == ".pl") {

		size_t	i = 0;
		size_t	size = response.size() - 2;

		response = executeCgi(obj.getURL());

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
	else {

		code = 204;
		response = "";
	}
	if (code == 500)
		response = readErrorsfiles(errorsFiles[code]);
}