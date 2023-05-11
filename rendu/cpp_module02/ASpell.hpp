#pragma once 

#include <iostream>
#include <string>
#include "ATarget.hpp"

class ATarget;

class ASpell{
    protected:
        std::string name;
        std::string effects;

        ASpell();
        ASpell(const ASpell &);
    public:

        virtual ~ASpell(){};
        ASpell(const std::string &, const std::string &);
        std::string const &getName() const;
        std::string const &getEffects() const;

        virtual ASpell *clone() = 0;
        void launch(const ATarget &) const;

};