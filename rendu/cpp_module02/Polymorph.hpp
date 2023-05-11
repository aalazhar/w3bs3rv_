#include "ASpell.hpp"

class Polymorph : public ASpell{
    private:

    public:
        Polymorph();
        virtual ASpell *clone();
};