#pragma once

#include "ATarget.hpp"

class ATarget;

class BrickWall : public ATarget{
    private:

    public:
        BrickWall();
        virtual ATarget *clone();
};