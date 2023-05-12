/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrisse <megrisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 18:37:50 by megrisse          #+#    #+#             */
/*   Updated: 2023/05/12 13:56:52 by megrisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <map>
#include "webServer.hpp"

class   Response  : public Req {
private :
	const	parserObject& 				_config;
	std::string							response_header;
	std::map<std::string, std::string>	_env;
	std::map<int, std::string>			errors;
	std::map<int, std::string>			errorsFiles;
	std::string							status_line;
	std::string							response_body;
	std::string							response;
	std::string							type;
	int									code;
	
public:
	Response(const	parserObject &);
	~Response();
	Response	&operator=(Req &);
	std::string	getResponse() {

		return (response);
	}
	//Methods 
	
	int	GetMethod(Req obj);
	int	DeletMethod(const Req &);
	int	PostMethod(const Req &);
	//Memeber Function
	std::string		getCgiPath();
	int				checkpath(std::string &);
	void			initEnvirement();
	void			initErrorMsgs();
	void			initErrorFiles();
	int				readcontent();
	std::string		readErrorsfiles(std::string);
	std::string		executeCgi(std::string );
	std::string		getResponseHeader();
	std::string		getStatusMsg(int code);
	std::string		getheaders();
	std::string		getContentType();
};