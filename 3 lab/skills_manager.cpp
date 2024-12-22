#include "skills_manager.h"


SkillsManager::SkillsManager(){
    creators.emplace_back(make_unique<DoubleDamageCreator>());
    creators.emplace_back(make_unique<BombingCreator>());
    creators.emplace_back(make_unique<ScannerCreator>());

    for (auto& creator : creators)
        this->skills.emplace_back(creator->create());

    random_device rd;
    default_random_engine engine(rd());
    shuffle(this->skills.begin(), this->skills.end(), engine);
}

ICreator& SkillsManager::getRandomCreator(){
    random_device rd;
    default_random_engine engine(rd());
    
    return *this->creators[engine() % this->creators.size()];
}

void SkillsManager::enqueueRandom(){
    this->skills.emplace_back(this->getRandomCreator().create());
}

void SkillsManager::enqueueRandomN(size_t n){
    for (size_t i = 0; i < n; ++i)
        this->skills.emplace_back(this->getRandomCreator().create());
}

unique_ptr<ISkill> SkillsManager::dequeue(){
    if(this->skills.size() == 0)
        throw NoSkillException();

    auto skill = move(this->skills[0]);
    this->skills.erase(this->skills.begin());

    return skill;
}

bool SkillsManager::useFront(InfoHolder& info_holder){
    return this->dequeue()->use(info_holder);
}

size_t SkillsManager::size(){
    return this->skills.size();
}

size_t SkillsManager::size() const {
    return this->skills.size();
}

const vector<unique_ptr<ISkill>>& SkillsManager::getSkills() const {
    return this->skills;
}

void SkillsManager::enqueue(size_t idx){
    this->skills.emplace_back(this->creators.at(idx)->create());
}