#pragma once

#include "i_attack_creator.h"
#include "double_damage_attack.h"


class DoubleDamageAttackCreator : public IAttackCreator {
    public:
        shared_ptr<IAttack> create();
};
