#include "double_damage_attack.h"


void DoubleDamageAttack::attack(PlayingField& playing_field, size_t x, size_t y, unsigned short int damage){
    playing_field.base_attack(x, y, damage*2);
    playing_field.setDefaultAttack();
}
