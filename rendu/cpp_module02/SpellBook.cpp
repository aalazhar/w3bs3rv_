#include "SpellBook.hpp"

void SpellBook::learnSpell(ASpell *p){
    if (map.find(p->getName()) == map.end())
        map.insert(std::make_pair(p->getName(), p->clone()));
}

void SpellBook::forgetSpell(std::string const &s){
    _IterMap it = this->map.find(s);
    if (it != this->map.end())
        delete it->second;
    map.erase(s);
}

ASpell* SpellBook::createSpell(std::string const &s){
    if (map.find(s) == map.end())
        return NULL;
    return map[s];
}

SpellBook::~SpellBook(){
    _IterMap it = this->map.begin();
    for(;it != this->map.end(); it++)
        delete it->second;
    this->map.clear();
}
