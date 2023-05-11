#include "Warlock.hpp"

Warlock::Warlock(const std::string &n, const std::string &t):name(n), title(t){
    std::cout << name << ": This looks like another boring day.\n";
}



std::string const &Warlock::getName() const  {return this->name;}

std::string const  &Warlock::getTitle() const  {return this->title;}

void Warlock::setTitle(const std::string &t) {this->title = t;}


Warlock::~Warlock(){
    _IterMap it = this->map.begin();
    for(;it != this->map.end(); it++)
        delete it->second;
    this->map.clear();
    std::cout << name << ": My job here is done!\n";    
}

void Warlock::introduce() const {std::cout << name << ": I am " << name << ", " << title << "!\n";};


void Warlock::learnSpell(ASpell *p){
    if (p != NULL)
        map.insert(std::make_pair(p->getName(), p->clone()));
}



void Warlock::forgetSpell(std::string s){
    _IterMap it = this->map.find(s);
    if (it != this->map.end())
        delete it->second;
    map.erase(s);
}



void Warlock::launchSpell(std::string name, ATarget const &rf){
    if (map.find(name) != map.end())
        map[name]->launch(rf);
}
