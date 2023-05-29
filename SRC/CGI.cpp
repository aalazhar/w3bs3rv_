/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrisse <megrisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 00:56:14 by aalazhar          #+#    #+#             */
/*   Updated: 2023/05/29 23:55:29 by megrisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/CGI.hpp"

CGI::CGI(){
	std::cout << "Default\n";
}

CGI::CGI(std::string fileName_, std::string method_, \
        std::string cntType_, std::string locaPath_, \
        std::string body_, std::string quiry_, int cntLenght_){
		
        this->fileName = "UTILS/" + fileName_;
        this->method = method_;
        this->cntType = cntType_;
        this->locaPath = locaPath_;
        this->body = body_;
        this->quiry = quiry_;
        this->cntLenght = cntLenght_;

		std::string code("200");
		vec.push_back("QUERY_STRING=" + this->quiry);
		vec.push_back("REQUEST_METHOD="+ this->method);
		vec.push_back("REDIRECT_STATUS=" + code);
		vec.push_back("SCRIPT_FILENAME=" + fileName);
		vec.push_back("CONTENT_TYPE=" + this->cntType);
		vec.push_back("BODY=" + this->body);
}

std::vector<std::string> CGI::executeCGI(){


	int fds[2];
	int status;
	std::vector<char> envp;
	std::vector<std::string> buff;
	
	std::cout << "here\n";
	const char *filePath = "/usr/bin/perl";
	char	*env[vec.size() + 1];
	for (std::size_t i = 0 ; i < vec.size(); i++)
		env[i] = const_cast<char *>(vec[i].c_str());
	env[this->vec.size()] = NULL;
	char *args[] = {(char *)filePath, (char *)this->fileName.c_str(), NULL};
	pipe(fds);
	pid_t pid = fork();
	if(pid == 0){

		close(fds[0]);
		dup2(fds[1], 1);
		execve(filePath, args, env);
		exit(1);
	}
	else {
		
		waitpid(pid, &status, 0);
		close(fds[1]);
		// char *buffer = new char[1024];
		memset(buffer, 0, 1024);
		int r = 0;
		r = read(fds[0], buffer, 1024);
		buffer[r] = '\0';
		buff.push_back(buffer);
		memset(buffer, 0, 1024);
		while (r){
			r = read(fds[0], buffer, 1024);
			buffer[r] = '\0';
			buff.push_back(buffer);
			memset(buffer, 0, 1024);
		}
		std::vector<std::string>::iterator it = buff.begin();
		while (it != buff.end()){
			std::cout << *it;
			it++;
		}
		close(fds[0]);
		// delete [] buffer;
	}
	return buff;
}

CGI::~CGI(){

	vec.clear();
}