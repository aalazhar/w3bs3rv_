#include "Warlock.hpp"

Warlock::Warlock(const std::string &n, const std::string &t):name(n), title(t){
    std::cout << name << ": This looks like another boring day.\n";
}



std::string const &Warlock::getName() const  {return this->name;}

std::string const  &Warlock::getTitle() const  {return this->title;}

void Warlock::setTitle(const std::string &t) {this->title = t;}


Warlock::~Warlock(){
    std::cout << name << ": My job here is done!\n";    
}

void Warlock::introduce() const {std::cout << name << ": I am " << name << ", " << title << "!\n";};


void Warlock::learnSpell(ASpell *p){
    if (p != NULL)
        this->spl.learnSpell(p);

}



void Warlock::forgetSpell(std::string s){
    this->spl.forgetSpell(s);
}



void Warlock::launchSpell(std::string name, ATarget const &rf){
    if (this->spl.createSpell(name) != NULL)
        this->spl.createSpell(name)->launch(rf);
}
