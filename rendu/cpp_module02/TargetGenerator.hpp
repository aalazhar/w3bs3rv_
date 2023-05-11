#pragma once

#include "ATarget.hpp"
#include <map>

class TargetGenerator{
    private:
        TargetGenerator(const TargetGenerator &);
        TargetGenerator &operator=(const TargetGenerator &);
        std::map<std::string, ATarget *> _map;
    public:
        TargetGenerator();
        ~TargetGenerator();
        void learnTargetType(ATarget*);
        void forgetTargetType(std::string const &);
        ATarget* createTarget(std::string const &);
};