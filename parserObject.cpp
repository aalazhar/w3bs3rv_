/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parserObject.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalazhar <aalazhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 18:59:54 by aalazhar          #+#    #+#             */
/*   Updated: 2023/03/30 22:41:38 by aalazhar         ###   ########.fr       */
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

int parserObject::check_whiteSpace(char c){
    if (c == ' ' || c == '\t')
        return (1);
    return (0);
}

int parserObject::check_check_check(std::string line){
    int i = 0;
    while (line[i] != '\n'){
        if (check_whiteSpace(line[i]))
            return (1);
        i++;
    }
    return (0);
}

// void parserObject::split_lines(std::string line, char sep){
//     int i = 0;
//     std::string res = "";

//     if (!line.size())
//         return;
//     while (((line[i] == ' ' || line[i] == '\t')) && line[i] != '\n' && i < line.size())
//         i++;
//     std::cout << " --- " << i << std::endl;
//     line.erase(0, i);
//     std::cout<< line << std::endl;
//     res = line;
//     i = 0;
//     while (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && i < line.size())
//         i++;
//     std::cout << " **** " << i << std::endl;
//     res = res.substr(0, i);
//     line = &res[i + 1];
//     std::cout<< res << std::endl;
//     std::cout << "" << line << std::endl;
//     i = 0;
//     while (check_whiteSpace(line[i])){
//         std::cout << line[i] << "-- ";
//         i++;
//     }
//     std::cout << " \n";
//     line.erase(0, i);
//     std::cout << "90900" << line << std::endl;
//     if (check_check_check(line))
//         split_lines(line, ' ');

// }

void parserObject::tabTurn_zero(int *tab){
    int x = -1;

    while (++x < 8)
        tab[x] = 0;
}

int parserObject::open_config_file(){
    std::ifstream lin(this->fileName);
    std::string line;
    struct loca loca;
    struct config cf;
    int pos = 0;
    int i = 0;
	int j = 0;
    int x = -1;
    int z = 0;
    int tab2[8];
    int tab[8];
    tabTurn_zero(tab);
    tabTurn_zero(tab2);
    
    if (!lin.is_open()){
        std::cout << "Can't open this file !" << std::endl;
        return (1);
    }
    if (lin.peek() == EOF){
        std::cout << "This file is empty !!\n";
        return (1);
    }
    if (lexical_analyser()){
        std::cout << "Syntax Error --- !" << std::endl;
        return (1);
    }
    while (getline(lin, line)){
        if (!strncmp(line.c_str(), "server", 6)){
            tabTurn_zero(tab);
            while (getline(lin, line)){
				j = 0;
				while (line[j] == '\t')
					j++;
				line.erase(0, j);
                split_lines(line, ' ', cf, tab);
                tabTurn_zero(tab2);
                if (!strncmp(line.c_str(), "location", 8)){
					if (locat_split_lines(line, ' ', loca, tab2)){
                        std::cout << "Invalid Directive !\n";
                        return (1);
                    }
                    while (getline(lin, line)){
						i = 0;
                        while (line[i] == '\t')
                            i++;
                        line.erase(0, i);
						if (locat_split_lines(line, ' ', loca, tab2)){
                            std::cout << "Invalid Directive !\n";
                            return (1);
                        }
                        if (line[0] == '}' && i == 1){
                            if (check_blocks_dirc(tab2)){
                                std::cout << "Syntax Error !!!" << std::endl;
                                return (1);
                            }
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
        if (check_blocks_dirc2(tab)){
            std::cout << "Syntax Error !!!" << std::endl;
            return (1);
        }
        this->server.push_back(cf);
        config_clean(cf);
    }

    std::cout << server[0].vect[0].l_path << std::endl;
    std::cout << server[0].err_p[0] << std::endl;
    // std::cout << server[0].err_p[1] << std::endl;

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
    conf.vect.clear();
    conf.err_p.clear();
    conf.a_meth.clear();
    conf.lsten.clear();
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
    loca.a_meth.clear();
}

int parserObject::check_blocks_dirc(int *tab){
    int i = 0;
    int flag = 0;

    while (i < 8){
        if (tab[i] > 1)
            return(1);
        if (tab[i] == 1)
            flag = 1;
        i++;
    }
    if (!flag)
        return (1);
    return (0);
}

int parserObject::check_blocks_dirc2(int *tab){
    int i = 0;
    int flag = 0;

    while (i < 8){
        if (tab[i] > 1)
            return(1);
        if (tab[i] == 1)
            flag = 1;
        i++;
    }
    if (!flag)
        return (1);
    return (0);
}

int parserObject::locat_split_lines(std::string line, char sep, struct loca& _location, int *tab){
    int i = 0;
    static int c = 0;
    std::string res;

    while (line[i] && line[i] != sep && line.size() > 0 && line[i] != '}')
        i++;
    res = line.substr(0, i); 
    if (res == "root"){
        _location.root = &line[i + 1];
        _location.root.erase(_location.root.size() - 1, _location.root.size());
        tab[0] += 1;
    }
    else if (res == "index"){
        _location.index = &line[i + 1];
        _location.index.erase(_location.index.size() - 1, _location.index.size());
        tab[1] += 1;
    }
    else if (res == "autoindex"){
        _location.autoIndex = &line[i + 1];
        _location.autoIndex.erase(_location.autoIndex.size() - 1, _location.autoIndex.size());
        tab[2] += 1;
    }
    else if (res == "deny"){
        _location.deny = &line[i + 1];
        _location.deny.erase(_location.deny.size() - 1, _location.deny.size());
        tab[3] += 1;
    }
    else if (res == "redirect"){
        _location.redirect = &line[i + 1];
        _location.redirect.erase(_location.redirect.size() - 1, _location.redirect.size());
        tab[4] += 1;
    }
    else if (res == "allowed_methods"){
        _location.allowed_m = &line[i + 1];
        _location.allowed_m.erase(_location.allowed_m.size() - 1, _location.allowed_m.size());
        split_method_lines(_location.allowed_m, conf, 1, _location);
        tab[5] += 1;
    }
    else if (res == "cgiExt"){
        _location.cgiExt = &line[i + 1];
        _location.cgiExt.erase(_location.cgiExt.size() - 1, _location.cgiExt.size());
        _location.cgi.push_back(_location.cgiExt);
        tab[6] = 1;
    }
    else if (res == "cgiPath"){
        _location.cgiPath= &line[i + 1];
        tab[7] += 1;
    }
    else if (res == "location"){
        _location.l_path= &line[i + 1];
        _location.l_path.erase(_location.l_path.size() - 2, _location.l_path.size());
    }
    else if (res != "root" && res != "index" && res != "autoindex" && res != "deny" && res != "redirect" && \
        res != "allowed_methods" && res != "cgiExt" && res != "cgiPath" && res != "}" && res.size() > 0)
        return (1);
    return (0);
}

void parserObject::split_lines(std::string line, struct config& conf){
    int i = 0;
    std::string res = "";
    std::string rex = "";
    while ((line[i] == ' ' || line[i] == '\t'))
        i++;
    line.erase(0, i);
    i = 0;
    while (line[i] && line[i] != ' ' && line[i] != '\t')
        i++;
    res = line;
    line = line.substr(0, i);
    res = &res[i + 1];
    int j= 0;
    while (res[j] && (res[j] == ' ' || res[j] == '\t') )
        j++;
    res.erase(0, j);
}

void parserObject::rm_blanks(std::string& line){
    int i = 0;
    while ((line[i] == ' ' || line[i] == '\t'))
        i++;
    line.erase(0, i);

}

void parserObject::split_method_lines(std::string line, struct config& conf, int rep, struct loca& loca){
    int i = 0;
    std::string res = "";
    std::string rex = "";
    rm_blanks(line);
    while (isalpha(line[i]) && line[i] != ' ' && line[i] != '\t'){
        if (isupper(line[i]))
            i++;
        else{
            std::cout << "Those values must be Uppercase !\n";
            exit (1);
        }
    }
    res = line;
    res = res.substr(0, i);
    if (!rep)
        conf.a_meth.push_back(res);
    else{
        loca.a_meth.push_back(res);
    }
    line.erase(0, i + 1);
    i = 0;
    if (line.size()){
        if (!rep)
            split_method_lines(line, conf, 0, loca);
        else
            split_method_lines(line, conf, 1, loca);
    }
}

void parserObject::split_listen_line(std::string line, struct config& conf){
    int i = 0;
    int j = 0;
    std::string addr = "";
    std::string port = "";

    rm_blanks(line);
    while (line[i] != ':'){
        if(isspace(line[i])){
            std::cout << "Invalid Syntax ADDRESS IP & PORT !\n";
            exit (1);
        }
        i++;
    }
    j = i;
    addr = line;
    addr = addr.substr(0, i);
    port = &line[i + 1];
    i = 0;
    while (i < port.size()){
        if (std::isalpha(port[i]) || isspace(port[i])){
            std::cout << "Invalid Port Number !\n";
            exit (1);
        }
        i++;
    }
    port.erase(port.size() - 1, port.size());
    conf.lsten.push_back(addr);
    conf.lsten.push_back(port);

}

void parserObject::split_lines(std::string line, char sep, struct config& conf, int *tab){
    int i = 0;
    std::string res = "";
    std::string tmp = "";
    struct loca loca;

    while (line[i] != ' ' && line.size() > 0 && line[i] != '}')
        i++;
    res = line.substr(0, i);
    if (res == "listen"){
        conf.listen = &line[i + 1];
        split_listen_line(&line[i + 1], conf);
        tab[0] += 1;
    }
    else if (res == "root"){
        conf.pRoot = &line[i + 1];
        conf.pRoot.erase(conf.pRoot.size() - 1, conf.pRoot.size());
        tab[1] += 1;
    }
    else if (res == "index"){
        conf.index = &line[i + 1];
        conf.index.erase(conf.index.size() - 1, conf.index.size());
        tab[2] += 1;
    }
    else if (res == "autoindex"){
        conf.autoIndex = &line[i + 1];
        conf.autoIndex.erase(conf.autoIndex.size() - 1, conf.autoIndex.size());
        tab[3] += 1;
    }
    else if (res == "upload"){
        conf.upload = &line[i + 1];
        conf.upload.erase(conf.upload.size() - 1, conf.upload.size());
        conf.uploads.push_back(conf.upload);
        tab[4] = 1;
    }
    else if (res == "error_page"){
        conf.error_page = &line[i + 1];
        conf.error_page.erase(conf.error_page.size() - 1, conf.error_page.size());
        split_lines(conf.error_page, conf);
        tab[5] = 1;
    }
    else if (res == "allowed_methods"){
        conf.allowed_m = &line[i + 1];
        
        conf.allowed_m.erase(conf.allowed_m.size() - 1, conf.allowed_m.size());
        split_method_lines(conf.allowed_m, conf, 0, loca);
        tab[6] = 1;
    }
    else if (res == "location")
        tab[7] = 1;
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
            if (line.size() > 0){
                if (strncmp(line.c_str(), "location", 8) && (line[line.size() - 1] != ';' || check_dup_char(line, ';')) && \
                    line[0] != '}'){
                    std::cout << "ERRRRRR \n";
                    return (1);
                }
            }
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