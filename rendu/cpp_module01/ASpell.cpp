#include "ASpell.hpp"

ASpell::ASpell(const std::string &n, const std::string &e):name(n), effects(e){

}

std::string const &ASpell::getName() const {return this->name;}
std::string const &ASpell::getEffects() const {return this->effects;}


void ASpell::launch(const ATarget &t) const {
    t.getHitBySpell(*this);
}