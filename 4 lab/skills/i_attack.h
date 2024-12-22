#pragma once

#include <memory>

#include "playing_field.h"


class IAttack{
    public:
        virtual ~IAttack() = default;
        virtual void attack(PlayingField& playing_field, size_t x, size_t y, unsigned short int damage) = 0;
};
