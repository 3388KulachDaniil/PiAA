#pragma once

#include "i_attack.h"


class SimpleAttack : public IAttack{
    public:
        void attack(PlayingField& playing_field, size_t x, size_t y, unsigned short int damage=1);
};
