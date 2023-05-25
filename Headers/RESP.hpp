/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RESP.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrisse <megrisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 17:12:46 by megrisse          #+#    #+#             */
/*   Updated: 2023/05/25 16:49:33 by megrisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Headers.hpp"
#include "parserObjectU.hpp"
#include "RequestClass.hpp"
#include "CGI.hpp"

class Res  : public Req{
private :

    struct config                       Conf;
    bool								autoInx;
    std::string							Querry;
	std::string							filePath;
	std::string							_Cgipath;
	std::string							status_line;
	std::map<int, std::string>			CodesMsgs;
	std::map<int, std::string>			errorsFiles;
	std::map<std::string, std::string>	mimeTypes;
    std::string							response;
	std::string							type;
	int									code;
	std::string							Date;
    size_t							    Size_send;
	bool								cgi;
    std::string							root;
	size_t								headers_size;
	size_t								file_size;
	std::vector<char>					_headers;
	std::vector<char>					fileData;

public :

	//
	void setSizesend(size_t n) {Size_send = n;}
	size_t	getSizeSend(){return Size_send;}
    Res(struct config, int, int);
	std::vector<char>	&getResp() {return _headers;}
	std::vector<char>	makeResponse();
    void    resetvalues();
	void	buildResponse();
	void	buildErrorResponse();
	void	buildNormalResponse();
	void	buildCGIResponse();
	void	getifQuerry(std::string &);
	void	readErrorsfiles(std::string);
	void	readContent();
	void	getHeadersRes();
	void	setMIME();
	void	initCodesMsgs();
	void	getDate();
	int		checkCgipath(std::string &path);
	void	mergeResponse();
	// void	sendResponse(int);
	void 	keventUP(int, int, int, int);
	void	GET();
	std::string	getStatusMsg(int);
};