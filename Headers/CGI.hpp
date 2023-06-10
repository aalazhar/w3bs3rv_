/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalazhar <aalazhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 00:54:32 by aalazhar          #+#    #+#             */
/*   Updated: 2023/06/10 00:29:37 by aalazhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP
 
#include <string>
#include <unistd.h>
#include <vector>
#include <map>
#include <iostream>

class CGI {
private:
    std::string fileName;
    std::string port;
    std::string method;
    std::string httpV;
    std::string cntType;
    std::string locaPath;
    std::string body;
    std::string quiry;
    std::string cgiPath;
    std::string cookie;
    bool        cgiErr;
    char buffer[1025];
    // char    *str;
    // std::vector<std::string> buff;
    std::vector<std::string> vec;
    int         cntLenght;
    CGI();

public :
    CGI(std::string fileName_, std::string method_, \
        std::string cntType_, std::string locaPath_, \
        std::string body_, std::string quiry_, int cntLenght_, std::string Cookie);
    std::vector<std::string> executeCGI();
    void printString();
    bool   getcgierr() {return cgiErr;};
    
    ~CGI();

};

// CGI obj("fileCGI.php", "GET", "HTTP/1.1 200 OK", "/path/to/location", "body", "myQuiry", 10);
// CGI::CGI(std::string fileName_, std::string method_, \
//         std::string cntType_, std::string locaPath_, \
//         std::string body_, std::string quiry_, int cntLenght_){
		
// 		int i = 0;
// 		std::map<std::string, std::string> envMap;
// 		std::map<std::string, std::string>::iterator it;
//         this->fileName = fileName_;
//         this->method = method_;
//         this->cntType = cntType_;
//         this->locaPath = locaPath_;
//         this->body = body_;
//         this->quiry = quiry_;
//         this->cntLenght = cntLenght_;

// 		// envMap.insert({"HOSTNAME", this->host});
// 		// envMap.insert(std::make_pair("HTTP_COOKIE", this->fileName ));
// 		envMap.insert(std::make_pair("QUERY_STRING", this->quiry));
// 		envMap.insert(std::make_pair("REQUEST_METHOD", this->method));
// 		envMap.insert(std::make_pair("SCRIPT_FILENAME", this->fileName));
// 		envMap.insert(std::make_pair("CONTENT_TYPE", this->cntType));
// 		envMap.insert(std::make_pair("BODY", this->body));

#endif