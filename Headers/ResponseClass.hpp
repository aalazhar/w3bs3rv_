/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RESP.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: megrisse <megrisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 17:12:46 by megrisse          #+#    #+#             */
/*   Updated: 2023/06/08 06:18:25 by megrisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Headers.hpp"
#include "parserObjectU.hpp"
#include "RequestClass.hpp"
#include "CGI.hpp"

#define REDIRECT 66

class Res  : public Req{
private :

    struct config                       Conf;
    bool								autoInx;
    bool								binary;
	bool								isLoc;
    std::string							Querry;
	std::string							path_to_upld;
	std::string							upld_file_name;
	std::string							upld_body;
	std::string							filePath;
	std::string							redirection_path;
	std::string							_Cgipath;
	std::string							cookie;
	std::string							status_line;
	std::map<int, std::string>			CodesMsgs;
	std::map<int, std::string>			errorsFiles;
	std::map<std::string, std::string>	mimeTypes;
	std::string							type;
	int									code;
	std::string							Date;
	std::string							index;
    size_t							    Size_send;
	bool								cgi;
    std::string							root;
	std::string							response_header;
	size_t								headers_size;
	size_t								file_size;
	std::vector<char>					_headers;
	std::vector<char>					fileData;
	std::vector<char>					Resp;
	std::map<std::string, std::string>	pairs;
	std::string							url_;

public :
    Res(struct config, int, int);
	~Res();
	size_t	getSizeSend();
	void 	setSizesend(size_t n) ;
	std::vector<char>	&getResp();
	//Headers
	void		setMIME();
	void		getHeadersRes();
	void		initCodesMsgs();
	void		getDate();
	void		mergeResponse();
	std::string	getStatusMsg(int);
	//Methods
	void	GET();
	void	POST();
	void	DELETE();
	//Methods Utils
	void		beginInPOST();
	void		getUpFname(std::string);
	std::string	getBoundry();
	std::string	getMimetype(std::string);
	void		getpathtoUp();
	void		CreateFile();
	void		buildResponse();
	void		initErrorFiles();
	void		buildErrorResponse();
	void		buildNormalResponse();
	void		buildCGIResponse();
	void		getifQuerry(std::string);
	int			checkCgipath(std::string);
	void		readErrorsfiles(std::string);
	void		readContent();
    void    	resetvalues();
	void		autoindex();
	void		getPairs();
	void		CreatepairsFiles();
	void		Generatename();
	void		Handl_encoded();
	bool		CheckIfLoction(std::string);
	bool		GetIfAutoIndex(std::string);
	bool		IfMatch(std::string);
	void		isloca();
	void		POST_CGI();
	bool		IfRoot(std::string);
	void 		splitUrl(std::string);
	bool		IfRedirection(std::string);
	void		BuildRedRes();
	void		SplitRed();
	void		GetCookie();
	bool		CheckMethodIfAllowed();
};