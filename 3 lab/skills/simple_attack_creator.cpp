#include "simple_attack_creator.h"


shared_ptr<IAttack> SimpleAttackCreator::create(){
    return make_shared<SimpleAttack>();
}
