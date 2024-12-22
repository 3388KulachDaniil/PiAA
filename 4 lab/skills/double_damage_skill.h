#pragma once

#include "i_skill.h"


class DoubleDamageSkill: public ISkill{
    public:
        bool use(InfoHolder& info_holder);
};
