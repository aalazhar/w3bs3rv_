/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrisse <megrisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 00:56:14 by aalazhar          #+#    #+#             */
/*   Updated: 2023/06/09 03:52:15 by megrisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/CGI.hpp"

CGI::CGI(){ ; }

CGI::CGI(std::string fileName_, std::string method_, \
        std::string cntType_, std::string locaPath_, \
        std::string body_, std::string quiry_, int cntLenght_, std::string Cookie_){
		
        this->fileName = fileName_;
        this->method = method_;
        this->cntType = cntType_;
        this->locaPath = locaPath_;
        this->body = body_;
        this->quiry = quiry_;
        this->cntLenght = cntLenght_;
		this->cookie = Cookie_;
		this->cgiErr = false;
		vec.clear();
		std::string code("200");
		if (this->method == "POST")
			this->quiry = this->body;
		vec.push_back(std::string("QUERY_STRING=" + this->quiry));
		vec.push_back(std::string("REQUEST_METHOD="+ this->method));
		vec.push_back(std::string("REDIRECT_STATUS=" + code));
		vec.push_back(std::string("SCRIPT_FILENAME=" + fileName));
		vec.push_back(std::string("CONTENT_TYPE=" + this->cntType));
		if (!cookie.empty())
			vec.push_back(std::string("HTTP_COOKIE=" + this->cookie));
		vec.push_back(std::string("BODY=" + this->body));
}

char** vectos(std::vector<std::string> &vec, char**env) {

	size_t i = 0;
	for (; i < vec.size(); i++)
		env[i] = const_cast<char *>(vec[i].c_str());
	env[vec.size()] = NULL;
	return env;
}

std::string	getdir(std::string path) {

	size_t pos = path.rfind('/');

	std::string dir = "";
	if (pos != std::string::npos)
		dir = path.erase(pos, path.size());
	return "../" + dir;
}

std::vector<std::string> CGI::executeCGI(){

	int fds[2];
	int status;
	std::vector<char> envp;
	std::vector<std::string> buff;
	const char *filePath;
	if (this->cntType == "php")
		filePath = "CGI-bin/php-cgi";
	else
		filePath = "/usr/bin/perl";
	char *args[] = {(char *)filePath, (char *)this->fileName.c_str(), NULL};
	pipe(fds);
	pid_t pid = fork();
	char	*env[vec.size() + 1];
	if(pid == 0){
		chdir(getdir(fileName).c_str());
		close(fds[0]);
		dup2(fds[1], 1);
		execve(filePath, args, vectos(vec, env));
		exit(1);
	}
	else {
		waitpid(pid, &status, 0);
		if (WIFEXITED(status)){
			if (WEXITSTATUS(status) != 0){
				this->cgiErr = true;
				return buff;
			}
		}
		close(fds[1]);
		int r = 0;
		r = read(fds[0], buffer, 1024);
		buffer[r] = 0;
		buff.push_back(buffer);
		while (r){
			r = read(fds[0], buffer, 1024);
			buffer[r] = 0;
			buff.push_back(buffer);
		}
		close(fds[0]);
	}
	return buff;
}

CGI::~CGI(){ ; }
