#include "simple_attack.h"


void SimpleAttack::attack(PlayingField& playing_field, size_t x, size_t y, unsigned short int damage){
    playing_field.base_attack(x, y, damage);
}
