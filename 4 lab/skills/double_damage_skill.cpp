#include "double_damage_skill.h"
#include "double_damage_attack_creator.h"


bool DoubleDamageSkill::use(InfoHolder& info_holder){
    info_holder.playing_field->setAttackType(DoubleDamageAttackCreator().create());
    return true;
}
