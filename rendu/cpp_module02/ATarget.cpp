#include "ATarget.hpp"

ATarget::ATarget(const std::string &t):type(t){

}

std::string &ATarget::getType(){return this->type;}

void ATarget::getHitBySpell(const ASpell &s) const{
    std::cout << type << " has been " << s.getEffects() << "!\n";
}
