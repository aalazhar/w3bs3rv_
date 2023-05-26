/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RESP.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrisse <megrisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 17:12:46 by megrisse          #+#    #+#             */
/*   Updated: 2023/05/26 02:00:51 by megrisse         ###   ########.fr       */
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
	std::string							type;
	int									code;
	std::string							Date;
    size_t							    Size_send;
	bool								cgi;
    std::string							root;
	std::string							response_header;
	size_t								headers_size;
	size_t								file_size;
	std::vector<char>					_headers;
	std::vector<char>					fileData;
	std::vector<char>					Resp;

public :

	//
	void setSizesend(size_t n) {Size_send = n;}
	size_t	getSizeSend(){return Size_send;}
    Res(struct config, int, int);
	std::vector<char>	&getResp() {return Resp;}
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
	void 	keventUP(int, int, int, int);
	void	GET();
	std::string	getStatusMsg(int);
	std::string	getMimetype(std::string);
	void	initErrorFiles();
};