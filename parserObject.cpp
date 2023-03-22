/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parserObject.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalazhar <aalazhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 18:59:54 by aalazhar          #+#    #+#             */
/*   Updated: 2023/03/22 21:18:17 by aalazhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parserObject.hpp"

parserObject::parserObject(std::string f_name){
    this->fileName = f_name;
}

void parserObject::print_directs_keys_val(std::vector<struct config> server){
    int i = 0;
    
    while (i < server.size()){
        std::cout << "------ DRCT------" << std::endl;
        std::cout << "root :" << server[i].pRoot << std::endl;
        std::cout << "indx :" << server[i].index << std::endl;
        std::cout << "lstn :" << server[i].listen << std::endl;
        std::cout << "auto :" << server[i].autoIndex << std::endl;
        std::cout << "allw :" << server[i].allowed_m << std::endl;
        std::cout << "errp :" << server[i].error_page << std::endl;
        std::cout << "uplo :" << server[i].upload << std::endl;
        std::cout << "------ DRCT------" << std::endl;
        print_location_directs(this->server[i].vect);
        i++;
    }

}

int parserObject::open_config_file(){
    std::ifstream lin(this->fileName);
    std::ifstream kin(this->fileName);
    std::string line;
    std::string key;
    struct loca loca;
    struct config cf;
    int pos = 0;
    int i = 0;
	int j = 0;
    
    if (!lin.is_open()){
        std::cout << "Can't open this file !" << std::endl;
        return (1);
    }
    if (lexical_analyser()){
        std::cout << "Syntax Error" << std::endl;
        return (1);
    }
    while (getline(lin, line)){
        if (!strncmp(line.c_str(), "server", 6)){
            while (getline(lin, line)){
				j = 0;
				while (line[j] == '\t')
					j++;
				line.erase(0, j);
                split_lines(line, ' ', cf);
                if (!strncmp(line.c_str(), "location", 8)){
					locat_split_lines(line, ' ', loca);
                    while (getline(lin, line)){
						i = 0;
                        while (line[i] == '\t')
                            i++;
                        line.erase(0, i);
						locat_split_lines(line, ' ', loca);
                        if (line[0] == '}' && i == 1){
							cf.vect.push_back(loca);
							clean_location_directs(loca);
                            break;
						}  
                    }
                }
                if (line[0] == '}' && j == 0){
                    break;
                }
            }
        }
        if (line.size() == 0)
            continue;
        this->server.push_back(cf);
        config_clean(cf);
        cf.vect.clear();
    }
    print_directs_keys_val(this->server);
    return (0);
}

void parserObject::config_clean(struct config& conf){
    conf.pRoot = "";
    conf.allowed_m = "";
    conf.autoIndex = "";
    conf.index = "";
    conf.error_page = "";
    conf.listen = "";
    conf.upload = "";
}

void parserObject::print_location_directs(std::vector<struct loca> vect){
    int i = 0;
    
    while (i < vect.size()){
        std::cout << "-----******{{{{ LOCA }}}}******-----" << "\n";
        std::cout << "root : " << vect[i].root << "\n";
        std::cout << "inde : " << vect[i].index << "\n";
        std::cout << "auto : " << vect[i].autoIndex << "\n";
        std::cout << "allo : " << vect[i].allowed_m << "\n";
        std::cout << "deny : " << vect[i].deny << "\n";
        std::cout << "redi : " << vect[i].redirect << "\n";
        std::cout << "cext : " << vect[i].cgiExt << "\n";
        std::cout << "cpat : " << vect[i].cgiPath << "\n";
        std::cout << "locp : " << vect[i].l_path << "\n";
        std::cout << "-----******{{{{ LOCA }}}}******-----" << "\n";
        i++;
    }
    
}

void parserObject::clean_location_directs(struct loca& loca){
    loca.root = "";
    loca.allowed_m = "";
    loca.index = "";
    loca.autoIndex = "";
    loca.deny = "";
    loca.redirect = "";
    loca.cgiExt = "";
    loca.cgiPath = "";
    loca.l_path = "";
}

void parserObject::locat_split_lines(std::string line, char sep, struct loca& _location){
    int i = 0;
    static int c = 0;
    std::string res;

    while (line[i] && line[i] != sep && line.size() > 0)
        i++;
    res = line.substr(0, i); 
    if (res == "root")
        _location.root = &line[i + 1];
    else if (res == "index")
        _location.index = &line[i + 1];
    else if (res == "autoindex")
        _location.autoIndex = &line[i + 1];
    else if (res == "deny")
        _location.deny = &line[i + 1];
    else if (res == "redirect")
        _location.redirect = &line[i + 1];
    else if (res == "allowed_methods")
        _location.allowed_m = &line[i + 1];
    else if (res == "cgiExt")
        _location.cgiExt = &line[i + 1];
    else if (res == "cgiPath")
        _location.cgiPath= &line[i + 1];
    else if (res == "location")
        _location.l_path= &line[i + 1];
}

void parserObject::split_lines(std::string line, char sep, struct config& conf){
    int i = 0;
    std::string res;

    while (line[i] != sep && line.size() > 0)
        i++;
    res = line.substr(0, i); 
    if (res == "listen")
        conf.listen = &line[i + 1];
    else if (res == "root")
        conf.pRoot = &line[i + 1];
    else if (res == "index")
        conf.index = &line[i + 1];
    else if (res == "autoindex")
        conf.autoIndex = &line[i + 1];
    else if (res == "upload")
        conf.upload = &line[i + 1];
    else if (res == "error_page")
        conf.error_page = &line[i + 1];
    else if (res == "allowed_methods")
        conf.allowed_m = &line[i + 1];
}

int parserObject::check_dup_char(std::string line, char c){
    int i = 0;
    int count = 0;

    while (line[i]){
        if (line[i] == c)
            count++;
        i++;
    }
    if (!count || count > 1)
        return (1);
    return (0);
}

int parserObject::lexical_analyser(){
    std::string line;
    std::ifstream lin(this->fileName);
    int i = 0;
    int j = 0;
    int y = 0;
    int count = 0;
    int pos = 0;
    int ps = 0;;

    if (!lin.is_open()){
        std::cout << "Can't open this file !" << std::endl;
        return (1);
    }
    while (getline(lin, line)){
        i = 0;
        while (line[i] == ' ' || line[i] == '\t')
            i++;
        line.erase(0, i);
        // std::cout << line << std::endl;
        if (line.size() == 0)
            continue;
        if (!strncmp(line.c_str(), "server", 6) && i != 0){
            std::cout << "Syntax Error 1!" << std::endl;
            return (1);
        }
        pos = line.find("{", 0);
        if (strncmp(line.c_str(), "server", 6) != 0 || pos != 7 || \
            line.size() != 8 || line[6] != ' '){
            std::cout << "Error 10\n";
            return (1);
        }
        while (getline(lin, line)){
            j = 0;
            while (line[j] == ' ' || line[j] == '\t')
                j++;
            line.erase(0, j);
            // std::cout << "*** " << line << std::endl;
            if (line[0] == '}' && j == 0 && !line.size())
                continue;
            if (j != 1 && line[0] != '}' && line.size() != 0){
                std::cout << "Error Nw\n";
                return (1);
            }
            if (!strncmp(line.c_str(), "location", 8) && line[line.size() - 1] == '{' && \
                (j == 4 || j == 1)){
                if (check_dup_char(line, '{') || line[8] != ' ')
                    return (1);
                while (getline(lin, line)){
                    y = 0;
                    std::cout << line << std::endl;
                    while (line[y] == ' ' || line[y] == '\t')
                        y++;
                    line.erase(0, y);
                    if (line.size() > 0){
                        if ((line[line.size() - 1] != ';' || check_dup_char(line, ';')) && \
                            line[0] != '}')
                            return (1);
                    }
                    if (y != 2 && line.size() != 0 && line[0] != '}'){
                        std::cout << "Syntax Error !7" << std::endl;
                        return (1);
                    }
                    if (line[0] == '}' && y == 1 && line.size() != 1){
                        std::cout << "Syntax Error 12\n";
                        return (1);
                    }
                    else if (line.find("{") != std::string::npos || \
                        (line.find("}") != std::string::npos && line.size() != 1)){
                        std::cout << "Syntax Error 13\n";
                        return (1);
                    }
                    else if (line[0] == '}' && y == 1 && line.size() == 1){
                        break;
                    }
                }
            }
            else if (line[0] == '}' && y == 1 && line.size() != 1){
                std::cout << "Syntax Error 16\n";
                return (1);
            }
            else if (line.find("{") != std::string::npos || \
                (line.find("}") != std::string::npos && line.size() != 1)){
                std::cout << "Syntax Error 15!\n";
                return (1);
            }
            if (line[0] == '}' && j == 0)
                break;
        }
    }
    return (0);
}

std::string parserObject::get_listen(){
    return (this->conf.listen);
}

std::string parserObject::get_error(){
    return (this->conf.error_page);
}

struct config parserObject::get_location_vect(){
    return (this->conf);
}

parserObject::~parserObject(){
    std::cout << "Destructer" << std::endl;
}