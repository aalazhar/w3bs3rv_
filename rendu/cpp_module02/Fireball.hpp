#include "ASpell.hpp"

class Fireball : public ASpell{
    private:

    public:
        Fireball();
        virtual ASpell *clone();
};