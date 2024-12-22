#pragma once

#include "i_attack_creator.h"
#include "simple_attack.h"


class SimpleAttackCreator : public IAttackCreator {
    public:
        shared_ptr<IAttack> create();
};
