#include "Fwoosh.hpp"

Fwoosh::Fwoosh() : ASpell("Fwoosh", "fwooshed") {
    
}

ASpell *Fwoosh::clone(){
    return new Fwoosh;
}