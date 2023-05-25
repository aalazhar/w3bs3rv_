/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrisse <megrisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 00:56:14 by aalazhar          #+#    #+#             */
/*   Updated: 2023/05/25 02:41:01 by megrisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/CGI.hpp"

CGI::CGI(){
	std::cout << "Default\n";
}

char **CGI::getEnv(){
	return (this->env);
}

// char *CGI::putEnvValues(std::string s1, std::string s2, std::string& s){
// 	// std::cout << "++++==" << s1 << std::endl;
// 	// std::cout << "++++==" << s2 << std::endl;
// 	// std::string merge = s1 + "=" + s2;
// 	std::string merge(s1);
// 	merge.append("=");
// 	merge.append(s2);
// 	// s = 
// 	// std::cout << merge << std::endl;
// 	// char *str = new char[len];
// 	char str[1024];
// 	strcpy(str, merge.c_str());
// 	// const char *str = merge.c_str();
// 	// std::cout << ")))))))" << str << std::endl;
// 	return (str);

// }

CGI::CGI(std::string fileName_, std::string method_, \
        std::string cntType_, std::string locaPath_, \
        std::string body_, std::string quiry_, int cntLenght_){
		
		int i = 0;
		std::map<std::string, std::string> envMap;
		std::map<std::string, std::string>::iterator it;
        this->fileName = "UTILS/" + fileName_;
        this->method = method_;
        this->cntType = cntType_;
        this->locaPath = locaPath_;
        this->body = body_;
        this->quiry = quiry_;
        this->cntLenght = cntLenght_;
		// this->buff = new char[1024];

		// envMap.insert({"HOSTNAME", this->host});
		// envMap.insert(std::make_pair("HTTP_COOKIE", this->fileName ));
		envMap.insert(std::make_pair("QUERY_STRING", this->quiry));
		envMap.insert(std::make_pair("REQUEST_METHOD", this->method));
		envMap.insert(std::make_pair("REDIRECT_STATUS", "200"));
		envMap.insert(std::make_pair("SCRIPT_FILENAME", fileName));
		envMap.insert(std::make_pair("CONTENT_TYPE", this->cntType));
		envMap.insert(std::make_pair("BODY", this->body));
		it = envMap.begin();
		this->env  = new char*[7];
		// it = envMap.find("HOSTNAME");
		// this->env[0] = putEnvValues("it->first", "it->second");
		// std::cout << str << std::endl;
		// std::cout << "--&--" << putEnvValues("it->first", "it->second") << std::endl;
		// std::cout << it->first << "=" << it->second << std::endl;
		std::vector<std::string> test;
		std::string s = "";
		while (it != envMap.end()){
			// (putEnvValues(it->first, it->second, s))
			s = it->first + "=" + it->second;
			this->env[i] = const_cast<char *>(s.c_str());
			// std::cout << "----- --- " << this->env[i] <<std::endl;
			it++;
			i++;
		}
		this->env[i] = NULL;
		// std::vector<std::string>::iterator fih;
		// for (fih = test.begin(); fih != test.end(); fih++) {
		// 	std::cout << *fih << std::endl;
		// }
		// for (it = envMap.begin(); it != envMap.end(); it++) {
		// 	std::cout << "fih fih fih " << this->env[i] << std::endl;
		// 	i++; 
		// }
		// this->env[i] = nullptr;
		// i = 0;
		// for (i = 0; 
		// while (env[i] != nullptr ){
			// std::cout << this->env[i] << std::endl;
			// i++;
		// }


		
}

std::vector<std::string> CGI::executeCGI(){


	int fds[2];
	int status;
	std::vector<char> envp;
	std::vector<std::string> buff;
	
	std::cout << "here\n";
	const char *filePath = "/Users/megrisse/Desktop/webserv/CGI-bin/php-cgi";
	char *args[] = {(char *)filePath, (char *)this->fileName.c_str(), NULL};
	pipe(fds);
	// std::cout << "hey\n";
	pid_t pid = fork();
	if(pid == 0){

		// int i = 0;
		// 	while (i < 9){
		// 		std::cout  << "######## " << this->env[i] << std::endl;
		// 		i++;
		// 	}
		close(fds[0]);
		// std::cout << "===== " << env[0] << std::endl;
		dup2(fds[1], 1);
		execve(filePath, args, this->env);
		exit(1);
	}
	else {
		
		waitpid(pid, &status, 0);
		close(fds[1]);
		char *buffer = new char[1024];
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
	}
	return buff;
}

CGI::~CGI(){
	delete [] env;
}