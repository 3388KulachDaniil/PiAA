#pragma once

#include <cxxabi.h>

#include "skills_manager.h"
#include "i_skills_view.h"

using namespace std;


template<typename EnumT>
class SkillsManagerView: public ISkillsView{
    public:
        void printSkills(SkillsManager* skills_manager, ostream& stream=cout){
            int status;
            for (auto &skill: skills_manager->getSkills())
                stream<<abi::__cxa_demangle(typeid(*skill.get()).name(), 0, 0, &status)<<static_cast<char>(EnumT::SKILL_DELIMITER);
        }
};