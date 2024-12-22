#pragma once

#include <memory>

#include "i_attack.h"


class IAttackCreator{
    public:
        virtual ~IAttackCreator() = default;
        virtual shared_ptr<IAttack> create() = 0;
};
