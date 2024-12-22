#include "game_control.h"


GameControl::GameControl() {
    this->setDefault();
}

void GameControl::setDefault(){
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

char GameControl::operator[](string command){
    if (this->commands.find(command) == this->commands.end())
        throw invalid_argument("Команда не найдена!");
    return this->commands[command];
}

vector<string> GameControl::split(const string &s, char delim) {
    vector<string> elems;
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

void GameControl::load(string filename){
    ifstream in(filename);
    if (!in)
        throw logic_error("Файл c управлением не найден!");
    
    stringstream buffer;
    buffer << in.rdbuf();
    string text = buffer.str();
    vector<string> lines = this->split(text, '\n');
    in.close();

    map<string, char> tmp_commands;
    vector<char> keys;
    for (size_t i = 0; i < lines.size(); ++i){
        regex r(R"(^help|load|save|yes|no|attack|print_skills:[a-zA-Z1-9]{1}$)");
        smatch m;
        if (regex_search(lines[i], m, r)){
            if (m.size() != 1)
                throw logic_error("Неверная строка в конфигурационном файле!");
            vector<string> command = this->split(lines[i], ':');
            if (tmp_commands.find(command[0]) != tmp_commands.end())
                throw logic_error("Команда не может быть повторена!");
            tmp_commands[command[0]] = command[1][0];
            for (auto key : keys)
                if (command[1][0] == key)
                    throw logic_error("Нельзя названачать различные команды одним и тем же символом!");
            keys.push_back(command[1][0]);
        }
    }
    if (keys.size() != 7)
        throw logic_error("Не все команды заданы!");
    
    vector<string> required_commands = {"help", "load", "save", "yes", "no", "attack", "print_skills"};
    for (auto command : required_commands)
        if (tmp_commands.find(command) == tmp_commands.end())
            throw logic_error("Команда " + command + " не задана!");

    this->commands = tmp_commands;
    this->reverse_commands.clear();
    for (auto key : required_commands){
        this->reverse_commands[tmp_commands[key]] = key;
    }
}


string GameControl::parseCommand(char command){
    if (this->reverse_commands.find(command) == this->reverse_commands.end())
        throw invalid_argument("Команда не найдена!");
    return this->reverse_commands[command];
}