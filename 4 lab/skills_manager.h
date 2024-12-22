#pragma once

#include <vector>
#include <algorithm>
#include <random>
#include <memory> // Added to include shared_ptr

#include "skills/i_creator.h"
#include "skills/bombing_creator.h"
#include "skills/double_damage_creator.h"
#include "skills/scanner_creator.h"
#include "no_skill_exception.h"
#include "info_holder.h"


class SkillsManager{
    public:
        SkillsManager();
        unique_ptr<ISkill> dequeue();
        void enqueueRandom();
        void enqueueRandomN(size_t n);
        bool useFront(InfoHolder& info_holder);
        size_t size();
        size_t size() const;
        const vector<unique_ptr<ISkill>>& getSkills() const;
        void enqueue(size_t idx);
    private:
        ICreator& getRandomCreator();

        vector<unique_ptr<ISkill>> skills;
        vector<unique_ptr<ICreator>> creators;
};