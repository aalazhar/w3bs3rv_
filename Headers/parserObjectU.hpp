#ifndef PARSEROBJECT_HPP
#define PARSEROBJECT_HPP

#include "Headers.hpp"

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
    std::vector <std::string> a_meth;
    std::vector <std::string> cgi;
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
    std::string s_name;
    std::vector <loca> vect;
    std::vector <std::string> s_names;
    std::vector <std::string> err_p;
    std::map <std::string, std::string> err_pm;
    std::vector <std::string> a_meth;
    std::vector <std::string> lsten;
    std::vector <std::string> uploads;
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
    int lexical_analyser();
    int check_dup_char(std::string line, char c);
    void print_directs_keys_val(std::vector<struct config> server);
    void clean_location_directs(struct loca& loca);
    void config_clean(struct config& conf);
    void print_location_directs(std::vector<struct loca> vect);
    int open_config_file();
    void split_lines(std::string line, char sep, struct config& conf, int *tab);
    int locat_split_lines(std::string line, char sep, struct loca& loca, int *tab);
    int check_whiteSpace(char c);
    int check_check_check(std::string line);
    void split_lines(std::string line, struct config& conf);
    void split_method_lines(std::string line, struct config& conf, int rep, struct loca& loca);
    void split_listen_line(std::string line, struct config& conf);
    void rm_blanks(std::string& line);
    void tabTurn_zero(int *tab, int size);
    void serverNameSplit(std::string& line, struct config& conf);
    void setDefaultsDirectives(struct config *cf, int *tab);
    void setDefaultsLocaDirectives(struct loca *loca, int *tab);
    std::vector<config> getServerConfig();
    std::vector<config>::iterator getItBegin();
    std::vector<config>::iterator getItend();
    int test_only_nl(std::string name);
    ~parserObject();

};

#endif