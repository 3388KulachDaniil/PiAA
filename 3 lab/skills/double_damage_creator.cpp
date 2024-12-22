#include "double_damage_creator.h"


unique_ptr<ISkill> DoubleDamageCreator::create(){
    return make_unique<DoubleDamageSkill>();
}
