#pragma once

#include <stdexcept>

using namespace std;


class NoSkillException : public runtime_error{
    public:
        NoSkillException() : runtime_error("No skills in queue"){}
};
