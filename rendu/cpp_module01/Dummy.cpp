#include "Dummy.hpp"

Dummy::Dummy() : ATarget("Target Practice Dummy") {
    
}

ATarget *Dummy::clone(){
    return new Dummy;
}