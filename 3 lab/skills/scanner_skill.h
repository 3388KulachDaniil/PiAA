#pragma once

#include "i_skill.h"


class ScannerSkill: public ISkill{
    public:
        bool use(InfoHolder& info_holder);
};
