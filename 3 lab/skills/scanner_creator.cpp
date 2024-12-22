#include "scanner_creator.h"


unique_ptr<ISkill> ScannerCreator::create(){
    return make_unique<ScannerSkill>();
}
