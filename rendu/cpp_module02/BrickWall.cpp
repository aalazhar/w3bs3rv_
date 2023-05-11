#include "BrickWall.hpp"



BrickWall::BrickWall() : ATarget("Inconspicuous Red-brick Wall"){

}

ATarget *BrickWall::clone() {return new BrickWall ;}