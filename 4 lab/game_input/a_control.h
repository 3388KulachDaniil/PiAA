#pragma once

#include <string>
#include <iostream>
#include <map>

using namespace std;



class AControl {
    public:
        AControl() {
            this->commands = {
            {"help", 'h'},
            {"load", 'l'},
            {"save", 's'},
            {"yes", 'y'},
            {"no", 'n'},
            {"print_skills", 'p'},
            {"attack", 'a'}
        };
        this->reverse_commands = {
            {'h', "help"},
            {'l', "load"},
            {'s', "save"},
            {'y', "yes"},
            {'n', "no"},
            {'p', "print_skills"},
            {'a', "attack"}
        };
        }
        ~AControl() {}
        virtual void load(string filename)=0;
        virtual void setDefault()=0;
        virtual char operator[](string command)=0;
        virtual string parseCommand(char command)=0;
    protected:
        map<string, char> commands;
        map<char, string> reverse_commands;
};