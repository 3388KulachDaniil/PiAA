#pragma once

#include "a_control.h"

#include <memory>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>

using namespace std;


class GameControl : public AControl {
    public:
        GameControl();
        ~GameControl() {};
        void load(string filename);
        void setDefault();
        char operator[](string command);
        string parseCommand(char command);
    private:
        vector<string> split(const string &s, char delim);
};