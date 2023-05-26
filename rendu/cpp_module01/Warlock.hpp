#pragma once


#include <iostream>
#include <string>
#include <map>
#include "ATarget.hpp"

class ATarget;
typedef std::map<std::string, ASpell *> _Map;
typedef std::map<std::string, ASpell *>::iterator _IterMap;

class Warlock{
    private:
        std::string name;
        std::string title;
        Warlock();
        Warlock(const Warlock&);
        Warlock &operator=(const Warlock &);

        _Map map;

    public:
        Warlock(const std::string &, const std::string &);
        ~Warlock();
        void introduce() const;
        std::string const &getName() const;
        std::string const  &getTitle() const;
        void setTitle(const std::string &t);

        void learnSpell(ASpell *);
        void forgetSpell(std::string  );
        void launchSpell(std::string , ATarget const &);

        // _IterMap getBegin(){return this->map.begin();}
        // _IterMap getEnd(){return this->map.end();}

};