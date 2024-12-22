#pragma once

#include "i_skill.h"


class BombingSkill: public ISkill{
    public:
        bool use(InfoHolder& info_holder);
};
