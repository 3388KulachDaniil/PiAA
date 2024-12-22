#include "double_damage_attack_creator.h"


shared_ptr<IAttack> DoubleDamageAttackCreator::create(){
    return make_shared<DoubleDamageAttack>();
}
