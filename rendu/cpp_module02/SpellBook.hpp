#pragma once

#include <map>
#include "ASpell.hpp"

typedef std::map<std::string, ASpell *> _Map;
typedef std::map<std::string, ASpell *>::iterator _IterMap;

class SpellBook{
    private:
        SpellBook &operator=(const SpellBook &);
        SpellBook(const SpellBook &);
    public:
        ~SpellBook();
        _Map map;
        SpellBook(){ ; };
        void learnSpell(ASpell*);
        void forgetSpell(std::string const &);
        ASpell* createSpell(std::string const &);
};