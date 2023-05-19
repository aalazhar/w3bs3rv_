/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hameur <hameur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 18:37:50 by megrisse          #+#    #+#             */
/*   Updated: 2023/05/18 20:13:27 by hameur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <stdio.h>
#include <ctime>
#include <unistd.h>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <map>

#include "RequestClass.hpp"
#include "parserObjectU.hpp"
#include "CGI.hpp"


// class Req;

class   Response  : public Req {
private :
	std::map<std::string, std::string>	_env;
	struct config						Locations;
	std::string							_Cgipath;
	std::string							AllowedM;
	bool								autoInx;
	std::string							Querry;
	std::string							filePath;
	std::string							status_line;
	std::string							response_header;
	std::string							response_body;
	std::map<int, std::string>			errors;
	std::map<int, std::string>			errorsFiles;
	std::string							response;
	std::vector<std::string>			_response;
	std::string							type;
	int									code;
	std::string							Date;
	size_t									r;
	bool								cgi;
	std::string							root;

public:
	size_t getR(){return this->r;}
	void setR(size_t R){this->r = R;}
	Response(struct config &);
	// Response(const	parserObject &);
	~Response();
	Response	&operator=(Req &);
	//Getters
	std::string	getStatusLine() {

		return (status_line);
	}
	std::string	getResHeaders() {

		return (response_header);
	}
	std::string	getResponse_body() {

		return (response_body);
	}
	//Methods 

	int	GetMethod(Req &obj);
	int	DeletMethod(Req &);
	int	PostMethod(Req &);
	//Memeber Function
	void			initResponse();
	int				getifQuerry(std::string &);
	int				checkCgipath(std::string &);
	int				checkpath(std::string &);
	void			initErrorMsgs();
	void			initErrorFiles();
	int				readcontent();
	std::string		readErrorsfiles(std::string);
	std::string		executeCgi(std::string );
	std::string		getResponseHeader();
	std::string		getStatusMsg(int code);
	std::string		getheaders(){return this->response_header;};
	std::string		getContentType();
	int 			makeResponse();
	void			getDate();
};