#include "bombing_creator.h"


unique_ptr<ISkill> BombingCreator::create(){
    return make_unique<BombingSkill>();
}
