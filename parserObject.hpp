/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parserObject.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalazhar <aalazhar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 18:59:23 by aalazhar          #+#    #+#             */
/*   Updated: 2023/03/23 23:34:10 by aalazhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEROBJECT_HPP
#define PARSEROBJECT_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <fcntl.h>

struct loca{
    std::string root;
    std::string index;
    std::string autoIndex;
    std::string allowed_m;
    std::string deny;
    std::string redirect;
    std::string cgiExt;
    std::string cgiPath;
    std::string l_path;
    int l_flag;

};

struct config{
    int port;
    std::string listen;
    std::string pRoot;
    std::string index;
    std::string autoIndex;
    std::string upload;
    std::string error_page;
    std::string allowed_m;
    std::vector<loca> vect;
};

struct sd{
    std::vector<config> server;
};

class parserObject{
private:
    std::string fileName;
    struct config conf;
    std::vector<config> server;
public:
    parserObject(std::string f_name);
    std::string get_listen();
    std::string get_error();
    int check_blocks_dirc(int *tab);
    int check_blocks_dirc2(int *tab);
    struct config get_location_vect();
    int check_loc_dirc(std::string line);
    int lexical_analyser();
    int check_dup_char(std::string line, char c);
    std::string strtrim(std::string line, std::ifstream& lin);
    std::string strtrim_loca(std::string line, std::ifstream& lin);
    void print_directs_keys_val(std::vector<struct config> server);
    void collect_directs_keys_val(std::ifstream& lin, std::string& line);
    void clean_location_directs(struct loca& loca);
    void config_clean(struct config& conf);
    void print_location_directs(std::vector<struct loca> vect);
    void collect_location_directs(std::ifstream& kin, std::string& line, struct loca& loca);
    int open_config_file();
    int check_server_direct();
    void split_lines(std::string line, char sep, struct config& conf, int *tab);
    void locat_split_lines(std::string line, char sep, struct loca& loca, int *tab);
    void print_text();
    ~parserObject();

};

#endif