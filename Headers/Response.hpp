/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrisse <megrisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 18:37:50 by megrisse          #+#    #+#             */
/*   Updated: 2023/05/23 20:19:43 by megrisse         ###   ########.fr       */
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
class webServ;

class   Response  : public Req {
private :
	std::map<std::string, std::string>	_env;
	struct config						Locations;
	std::string							_Cgipath;
	bool								autoInx;
	std::string							Querry;
	std::string							filePath;
	std::string							status_line;
	std::string							response_header;
	std::string							response_body;
	std::map<int, std::string>			errors;
	std::map<int, std::string>			errorsFiles;
	std::string							response;
	std::string							type;
	int									code;
	std::string							Date;
	size_t									r;
	bool								cgi;
	std::string							root;
	size_t								headers_size;
	size_t								file_size;
	std::vector<char>					_headers;
	std::vector<char>					fileData;

public:
	void setR(size_t R){this->r = R;}
	void setLocation(struct config &server);
	Response(struct config &, int, int);
	// Response(const	parserObject &);
	~Response();
	Response	&operator=(Req &);
	//Getters
	int		getCode() {return this->code;}
	size_t getR(){return this->r;}
	size_t getFileSize(){return file_size;}
	size_t getHeadersSize(){return headers_size;}
	std::vector<char>	&getFileData() {return fileData;};
	std::string	getStatusLine() {

		return (status_line);
	}
	std::string	getResHeaders() {

		return (response_header);
	}
	std::string	getResponse_body() {

		return (response_body);
	}
	std::string		getheaders(){return this->response_header;};
	//Methods 

	int	GetMethod(Req &obj);
	int	DeletMethod(Req &);
	int	PostMethod(Req &);
	//Memeber Function
	int				checkmethod(std::string);
	void			buildResponse(Req &, int);
	void			sendResponse(int, int);
	void			initResponse();
	int				getifQuerry(std::string &);
	int				checkCgipath(std::string &);
	int				checkpath(std::string &);
	void			initErrorMsgs();
	void			initErrorFiles();
	int				readcontent();
	void			readErrorsfiles(std::string);
	std::string		executeCgi(std::string );
	std::string		getResponseHeader();
	std::string		getStatusMsg(int code);
	std::string		getContentType();
	void 			makeResponse(Req &, int);
	void			getDate();
	void			resetvalues();
	void			Readimage(std::string);
	void			ReadFile(std::string);
	void			printvector(std::vector<char> vec, int);
	void			sendHeaders(int, std::string);
	void			sendBody(int, char *, size_t);
	void			keventUP(int, int, int, int);
	std::vector<char> convertTocharVec(std::vector<std::string> cgiBuff);
};