#include "../Headers/Headers.hpp"
#include "../Headers/parserObjectU.hpp"

parserObject::parserObject(std::string f_name){
    this->fileName = f_name;
}

void parserObject::print_directs_keys_val(std::vector<struct config> server){
    size_t i = 0;
    
    while (i < server.size()){
        // std::cout << "------ DRCT------" << std::endl;
        // std::cout << "root :" << server[i].pRoot << std::endl;
        // std::cout << "indx :" << server[i].index << std::endl;
        // std::cout << "lstn :" << server[i].listen << std::endl;
        // std::cout << "auto :" << server[i].autoIndex << std::endl;
        // std::cout << "allw :" << server[i].allowed_m << std::endl;
        // std::cout << "errp :" << server[i].error_page << std::endl;
        // std::cout << "uplo :" << server[i].upload << std::endl;
        // std::cout << "Vectors ----------------------------\n";
        // std::cout << "allw :" << server[i].a_meth[0] << std::endl;
        // std::cout << "errp :" << server[i].err_p[0] << std::endl;
        // std::cout << "uplo :" << server[i].uploads[0] << std::endl;
        // std::cout << "------ DRCT------" << std::endl;
        // print_location_directs(this->server[i].vect);
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

void parserObject::tabTurn_zero(int *tab, int size){
    int x = -1;

    while (++x < size)
        tab[x] = 0;
}

int parserObject::test_only_nl(std::string name){
    std::ifstream rin(name);
    std::string line = "";
    int flag = 0;

    while (getline(rin, line)){
        if (line.empty())
            continue;
        else{
            flag = 1;
            break;
        }
    }
    if (flag)
        return (1);
    return (0);
}

int parserObject::open_config_file(){
    std::ifstream lin(this->fileName);
    std::string line;
    struct loca loca;
    struct config cf;
    // int pos = 0;
    int i = 0;
	int j = 0;
    // int x = -1;
    // int z = 0;
    int tab2[8];
    int tab[9];
    tabTurn_zero(tab, 9);
    tabTurn_zero(tab2, 8);
    if (!lin.is_open())
        throw (std::invalid_argument("Can't open this file ! : " + this->fileName));
    if (!test_only_nl(this->fileName))
        throw (std::invalid_argument("This file is empty !!"));
    if (lin.peek() == EOF)
        throw (std::invalid_argument("This file is empty !!"));
    if (lexical_analyser())
        throw (std::invalid_argument("Syntax Error --- !"));
    while (getline(lin, line)){
        if (!strncmp(line.c_str(), "server", 6)){
            tabTurn_zero(tab2, 8);
            while (getline(lin, line)){
				j = 0;
				while (line[j] == '\t')
					j++;
				line.erase(0, j);
                split_lines(line, ' ', cf, tab2);
                tabTurn_zero(tab, 9);
                if (!strncmp(line.c_str(), "location", 8)){
					if (locat_split_lines(line, ' ', loca, tab))
                        throw (std::invalid_argument("Invalid Directive !"));
                    while (getline(lin, line)){
						i = 0;
                        while (line[i] == '\t')
                            i++;
                        line.erase(0, i);
						if (locat_split_lines(line, ' ', loca, tab))
                            throw (std::invalid_argument("Invalid Directive !!"));
                        if (line[0] == '}' && i == 1){
                            if (check_blocks_dirc(tab))
                                throw (std::invalid_argument("Syntax Error !!"));
                            setDefaultsLocaDirectives(&loca, tab);
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
        if (check_blocks_dirc2(tab2)){
            throw (std::invalid_argument("Syntax Error !!!"));
        }
        setDefaultsDirectives(&cf, tab2);
        this->server.push_back(cf);
        config_clean(cf);
    }

    return (0);
}

void parserObject::config_clean(struct config& conf){
    conf.pRoot = "";
    conf.s_name = "";
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
    conf.s_names.clear();
}

void parserObject::print_location_directs(std::vector<struct loca> vect){
    size_t i = 0;

    while (i < vect.size()){
        // std::cout << "-----******{{{{ LOCA }}}}******-----" << "\n";
        // std::cout << "root : " << vect[i].root << "\n";
        // std::cout << "inde : " << vect[i].index << "\n";
        // std::cout << "auto : " << vect[i].autoIndex << "\n";
        // std::cout << "allo : " << vect[i].allowed_m << "\n";
        // std::cout << "deny : " << vect[i].deny << "\n";
        // std::cout << "redi : " << vect[i].redirect << "\n";
        // std::cout << "cext : " << vect[i].cgiExt << "\n";
        // std::cout << "cpat : " << vect[i].cgiPath << "\n";
        // std::cout << "locp : " << vect[i].l_path << "\n";
        // std::cout << "-----******{{{{ LOCA }}}}******-----" << "\n";
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
void parserObject::setDefaultsLocaDirectives(struct loca *loca, int *tab){
    // int tab[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int i = 0;

    while (i < 9){

        if (tab[i] == 0) {

            switch (i) {

                case 0:
            	    loca->root = "Default/root/path";
            	    break;
            	case 1:
            	    // loca->index = "index.html";
            	    break;
            	case 2:
            	    loca->autoIndex = "off";
            	    break;
            	case 3:
            	    loca->deny = "all";
            	    break;
            	case 4:
            	    // loca->redirect = "http//:DefaultUrl";
            	    break;
            	case 5:
					loca->a_meth.push_back("GET");
					loca->a_meth.push_back("POST");
					loca->a_meth.push_back("DELETE");
            	    break;
            	case 6:
            	    loca->cgi.push_back("CGI Extension");
            	    break;
            	case 7:
            	    loca->cgi.push_back("CGI Path");
            	    break;
            	case 8:
                    loca->l_path = "root/";
            	    break;
	
            	default:
            	    break;
            }
        }
		i++;
    }
}

void parserObject::setDefaultsDirectives(struct config *cf, int *tab){


    int i = 0;
    // int y = 0;
    // int len = server.size();
    // while (y < len){
    i = 0;
    while (i < 7){
        if (tab[i] == 0){
            switch (i)
            {
                case 0 :
                    cf->pRoot = "";
                    break;
                case 1 :
                    break;
                case 2 :
                    break;
                case 3 :
                    break;
                case 4 :
                    break;
                case 5 :
                    break;
                case 6 :
                    cf->a_meth.push_back("GET");
                    cf->a_meth.push_back("POST");
                    cf->a_meth.push_back("DELETE");
                    break;
                default:
                    break;
            }
        }
    i++;
    }
    if (tab[7] == 1){

    }
}

int parserObject::check_blocks_dirc(int *tab){
    int i = 0;
    int flag = 0;

    while (i < 9){
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
    // int flag = 0;
    // static int c = 0;
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
    else if (res == "return"){
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
        tab[6] += 1;
    }
    else if (res == "cgiPath"){
        _location.cgiPath= &line[i + 1];
        _location.cgi.push_back(_location.cgiPath);
        tab[7] += 1;
    }
    else if (res == "location"){
        _location.l_path= &line[i + 1];
        _location.l_path.erase(_location.l_path.size() - 2, _location.l_path.size());
        if (_location.l_path[0] == ' ')
            return(1);
        tab[8] += 1;
    }
    else if (res != "root" && res != "index" && res != "serever_name" && res != "autoindex" && res != "deny" && res != "redirect" && \
        res != "allowed_methods" && res != "cgiExt" && res != "cgiPath" && res != "}" && res.size() > 0)
        return (1);
    return (0);
}

void parserObject::split_lines(std::string line, struct config& conf){
    int i = 0;
    std::string res = "";
    std::string code = "";
    while ((line[i] == ' ' || line[i] == '\t'))
        i++;
    line.erase(0, i);
    i = 0;
    while (line[i] && line[i] != ' ' && line[i] != '\t')
        i++;
    res = line;
    code = line.substr(0, i);
    res = &res[i + 1];
    int j= 0;
    while (res[j] && (res[j] == ' ' || res[j] == '\t') )
        j++;
    res.erase(0, j);
    conf.err_p.push_back(code);
    conf.err_p.push_back(res);
    // std::cout << code << std::endl;
    // std::cout << res << std::endl;
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
            std::cerr << "Those values must be Uppercase !\n";
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
    size_t i = 0;
    size_t j = 0;
    std::string addr = "";
    std::string port = "";

    rm_blanks(line);
    while (line[i] != ':'){
        if(isspace(line[i])){
            std::cerr << "Invalid Syntax ADDRESS IP & PORT !\n";
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
            std::cerr << "Invalid Port Number !\n";
            exit (1);
        }
        i++;
    }
    port.erase(port.size() - 1, port.size());
    conf.lsten.push_back(addr);
    conf.lsten.push_back(port);

}

void parserObject::split_lines(std::string line, char sep, struct config& conf, int *tab){
    size_t i = 0;
    (void)sep;
    std::string res = "";
    std::string tmp = "";
    struct loca loca;

    // while (line.c_str()[i] != ' ' && line.size() > 0 && line.c_str()[i] != '}') {
    //     std::cout << "line[i] == " << line.c_str()[i] << " i == " << i << std::endl; 
    //     i++;
    // }
    for (; line[i] != ' ' && line.size() > i && line[i] != '}'; i++);
    res = line.substr(0, i);
    if (res == "listen"){
        conf.listen = &line[i + 1];
        split_listen_line(&line[i + 1], conf);
        tab[1] += 1;
    }
    else if (res == "root"){
        conf.pRoot = &line[i + 1];
        conf.pRoot.erase(conf.pRoot.size() - 1, conf.pRoot.size());
        tab[0] += 1;
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
    else if (res == "server_name"){
        conf.s_name = &line[i + 1];
        conf.s_name.erase(conf.s_name.size() - 1, conf.s_name.size());
        serverNameSplit(conf.s_name, conf);
        tab[8] += 1;
    }
}

void parserObject::serverNameSplit(std::string& line, struct config& conf){
    size_t i = 0;
    std::string res = line;
    while (i < line.size()){
        if (line[i] == ' ')
            break;
        i++;
    }
    res = res.substr(0, i);
    line = line.erase(0, i + 1);
    conf.s_names.push_back(res);
    if (line.size())
        serverNameSplit(line, conf);
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
    // int count = 0;
    int pos = 0;
    // int ps = 0;
    int flag = 0;
    

    if (!lin.is_open()){
        std::cerr << "Can't open this file !" << std::endl;
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
            std::cerr << "Syntax Error 1!" << std::endl;
            return (1);
        }
        pos = line.find("{", 0);
        if (strncmp(line.c_str(), "server", 6) != 0 || pos != 7 || \
            line.size() != 8 || line[6] != ' '){
            std::cerr << "Error 10\n";
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
                    std::cerr << "ERRRRRR \n";
                    return (1);
                }
            }
            if (line[0] == '}' && j == 0 && !line.size())
                continue;
            if (j != 1 && line[0] != '}' && line.size() != 0){
                std::cerr << "Error Nw\n";
                return (1);
            }
            flag = 0;
            if (!strncmp(line.c_str(), "location", 8) && line[line.size() - 1] == '{' && \
                (j == 4 || j == 1)){
                if (check_dup_char(line, '{') || line[8] != ' ')
                    return (1);
                flag = 1;
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
                        std::cerr << "Syntax Error !7" << std::endl;
                        return (1);
                    }
                    if (line[0] == '}' && y == 1 && line.size() != 1){
                        std::cerr << "Syntax Error 12\n";
                        return (1);
                    }
                    else if (line.find("{") != std::string::npos || \
                        (line.find("}") != std::string::npos && line.size() != 1)){
                        std::cout << "Syntax Error 13\n";
                        return (1);
                    }
                    else if (line[0] == '}' && y == 1 && line.size() == 1 && flag == 1){
                        break;
                    }
                }
            }
            else if (line[0] == '}' && y == 1 && line.size() != 1){
                std::cerr << "Syntax Error 16\n";
                return (1);
            }
            else if (line.find("{") != std::string::npos || \
                (line.find("}") != std::string::npos && line.size() != 1)){
                std::cerr << "Syntax Error 15!\n";
                return (1);
            }
            if (line[0] == '}' && j == 1 && !flag)
                return (1);
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

parserObject::~parserObject(){ ; }

std::vector<config> parserObject::getServerConfig(){
    return (this->server);
}

std::vector<config>::iterator parserObject::getItBegin(){return this->server.begin();}
std::vector<config>::iterator parserObject::getItend(){return this->server.end();}