#pragma once

#include "skills_manager.h"


class ISkillsView{
    public:
        ~ISkillsView()=default;
        virtual void printSkills(SkillsManager* skills_manager, ostream& stream=cout)=0;
};