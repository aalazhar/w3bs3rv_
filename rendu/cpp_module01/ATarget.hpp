#pragma once

#include <iostream>
#include <string>
#include "ASpell.hpp"

class ASpell;

class ATarget{
    private:
        std::string type;
    public:
        virtual ~ATarget(){};
        ATarget(const std::string &);
        std::string &getType();
        virtual ATarget* clone() = 0;
        void getHitBySpell(const ASpell &) const;




};