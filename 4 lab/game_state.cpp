#include "game_state.h"
#include "md5.h"


GameState::GameState(
    shared_ptr<PlayingField> player_field, shared_ptr<PlayingField> enemy_field,
    shared_ptr<ShipsManager> player_ships, shared_ptr<ShipsManager> enemy_ships
):  player_field(player_field), enemy_field(enemy_field), player_ships(player_ships),
    enemy_ships(enemy_ships), info_holder(enemy_field, enemy_ships), skills_manager(SkillsManager()) {}


vector<pair<size_t, size_t>> GameState::getShipPosition(Ship& ship, PlayingField& field) const{
    vector<pair<size_t, size_t>> res;
    for (size_t x = 0; x < field.getWidth(); ++x){
        for (size_t y = 0; y < field.getHeight(); ++y){
            if (!field.getCell(x, y).isEmpty() && &field.getCell(x, y).getSegment() == &ship[0]){
                for (size_t i = 0; i < ship.size(); ++i){
                    if (ship.getOrientation() == Ship::Orientation::VERTICAL)
                        res.push_back(make_pair(x, y + i));
                    else
                        res.push_back(make_pair(x + i, y));
                }
            }
        }
    }
    return res;
}


string GameState::serializeShips(ShipsManager& ships_manager, PlayingField& field) const{
    string s = to_string(ships_manager.size()) + "\n";
    for (size_t i = 0; i < ships_manager.size(); ++i){
        s += to_string(ships_manager[i].size()) + " ";
        s += to_string(static_cast<int>(ships_manager[i].getOrientation())) + "|";
        vector<pair<size_t, size_t>> coords = this->getShipPosition(ships_manager[i], field);
        for (size_t j = 0; j < coords.size(); ++j)
            s += to_string(coords[j].first) + "," + to_string(coords[j].second) + "," + to_string(static_cast<int>(ships_manager[i][j].getState())) + " ";
        s += "\n";
    }
    return s;
}


string GameState::serializeField(PlayingField& field) const {
    string s = to_string(field.getWidth()) + " " + to_string(field.getHeight()) + "\n";
    for (size_t y = 0; y < field.getHeight(); ++y) {
        for (size_t x = 0; x < field.getWidth(); ++x) {
            s += to_string(static_cast<int>(field.getCell(x, y).getState())) + " ";
        }
        s += "\n";
    }
    return s;
}


string GameState::serializeSkills(const SkillsManager& skills_manager) const {
    int status;
    char* demangled;
    string s = to_string(skills_manager.size()) + "\n";
    for (const auto& skill : skills_manager.getSkills()) {
        demangled = abi::__cxa_demangle(typeid(*skill.get()).name(),0,0,&status);
        s += string(demangled) + "\n";
    }
    return s;
}


ostream& operator <<(ostream& os, const GameState& game_state){
    string s = "";

    s += game_state.serializeShips(*game_state.player_ships, *game_state.player_field);
    s += game_state.serializeShips(*game_state.enemy_ships, *game_state.enemy_field);

    s += game_state.serializeField(*game_state.player_field);
    s += game_state.serializeField(*game_state.enemy_field);

    s += game_state.serializeSkills(game_state.skills_manager);

    s += to_string(game_state.info_holder.x) + " " + to_string(game_state.info_holder.y) + "\n";

    s += to_string(game_state.is_player_turn) + "\n";
    s += to_string(game_state.is_game_started) + "\n";

    os << s << md5(s);
    return os;
}

vector<string> GameState::split(const string &s, char delim) {
    vector<string> elems;
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<tuple<size_t, Ship::Orientation, vector<tuple<size_t, size_t, Segment::State>>>> GameState::readShips(vector<string>& lines, size_t& j){
    vector<tuple<size_t, Ship::Orientation, vector<tuple<size_t, size_t, Segment::State>>>> ships;
    for (size_t i = j; i < stoi(lines[j - 1]) + j; ++i) {
        vector<string> s = split(lines[i], '|');
        size_t len = stoi(split(s[0], ' ')[0]);
        Ship::Orientation orientation = static_cast<Ship::Orientation>(stoi(split(s[0], ' ')[1]));
        vector<tuple<size_t, size_t, Segment::State>> segments;
        if (s.size() == 2){
            vector<string> segments_str = split(s[1], ' ');
            for (auto segment_str : segments_str) {
                vector<string> data = split(segment_str, ',');
                segments.push_back(make_tuple(stoi(data[0]), stoi(data[1]), static_cast<Segment::State>(stoi(data[2]))));
            }
        }
        ships.push_back(make_tuple(len, orientation, segments));
    }
    j += stoi(lines[j - 1]) + 1;
    return ships;
}

void GameState::updateFieldWithShips(PlayingField& field, ShipsManager& ships_manager, vector<tuple<size_t, Ship::Orientation, vector<tuple<size_t, size_t, Segment::State>>>>& ships, vector<string>& lines, size_t& j) {
    for (size_t k = 0; k < ships.size(); ++k){
        auto ship = ships[k];
        Ship::Orientation orientation = get<1>(ship);
        vector<tuple<size_t, size_t, Segment::State>> segments = get<2>(ship);
        if (segments.size() > 0){
            field.addShip(get<0>(segments[0]), get<1>(segments[0]), ships_manager[k], orientation);
            for (size_t i = 0; i < segments.size(); ++i)
                field.getCell(get<0>(segments[i]), get<1>(segments[i])).getSegment().setState(get<2>(segments[i]));
        }
    }
    for (size_t y = 0; y < field.getHeight(); ++y) {
        vector<string> states = split(lines[j + y], ' ');
        for (size_t x = 0; x < field.getWidth(); ++x) {
            field.getCell(x, y).setState(static_cast<Cell::ViewState>(stoi(states[x])));
        }
    }
    j += stoi(lines[j - 1]);
    j++;
}

istream& operator >>(istream& is, GameState& game_state){
    stringstream buffer;
    buffer << is.rdbuf();
    string line = buffer.str();
    vector<string> lines = game_state.split(line, '\n');
    if (lines.size() < 2)
        throw invalid_argument("Invalid save file");

    string save = "";
    for (size_t i = 0; i < lines.size() - 1; ++i)
        save += lines[i] + "\n";

    if (md5(save) != lines[lines.size() - 1])
        throw invalid_argument("Save file has been modified");
    
    size_t j = 1;
    vector<tuple<size_t, Ship::Orientation, vector<tuple<size_t, size_t, Segment::State>>>> player_ships = game_state.readShips(lines, j);
    vector<tuple<size_t, Ship::Orientation, vector<tuple<size_t, size_t, Segment::State>>>> enemy_ships = game_state.readShips(lines, j);
    vector<unsigned short int> player_ship_lens;
    vector<unsigned short int> enemy_ship_lens;
    for (auto ship : player_ships)
        player_ship_lens.push_back(get<0>(ship));
    for (auto ship : enemy_ships)
        enemy_ship_lens.push_back(get<0>(ship));
    game_state.player_ships = make_shared<ShipsManager>(ShipsManager(player_ship_lens));
    game_state.enemy_ships = make_shared<ShipsManager>(ShipsManager(enemy_ship_lens));
    vector<string> s = game_state.split(lines[j - 1], ' ');
    game_state.player_field = make_shared<PlayingField>(PlayingField(stoi(s[0]), stoi(s[1])));
    game_state.updateFieldWithShips(*game_state.player_field, *game_state.player_ships, player_ships, lines, j);
    s = game_state.split(lines[j - 1], ' ');
    game_state.enemy_field = make_shared<PlayingField>(PlayingField(stoi(s[0]), stoi(s[1])));
    game_state.updateFieldWithShips(*game_state.enemy_field, *game_state.enemy_ships, enemy_ships, lines, j);
    game_state.player_ships->getNewDeadsCount();
    game_state.enemy_ships->getNewDeadsCount();
    size_t skills_count = stoi(lines[j - 1]);
    game_state.skills_manager = SkillsManager();
    size_t base_skills_count = game_state.skills_manager.size();
    for (size_t i = 0; i < base_skills_count; ++i)
        game_state.skills_manager.dequeue();
    vector<size_t> skills;
    for (size_t i = 0; i < skills_count; ++i){
        if (lines[i + j] == "ScannerSkill"){
            skills.push_back(2);
        }
        else if (lines[i + j] == "BombingSkill"){
            skills.push_back(1);
        }
        else if (lines[i + j] == "DoubleDamageSkill"){
            skills.push_back(0);
        }
    }
    for (size_t i = 0; i < skills.size(); ++i){
        game_state.skills_manager.enqueue(skills[i]);
    }
    j += skills_count + 1;
    s = game_state.split(lines[j - 1], ' ');
    game_state.info_holder.x = static_cast<size_t>(stoi(s[0]));
    game_state.info_holder.y = static_cast<size_t>(stoi(s[1]));
    game_state.info_holder.playing_field = game_state.enemy_field;
    game_state.info_holder.ships_manager = game_state.enemy_ships;
    game_state.is_game_started = static_cast<bool>(stoi(lines[j]));
    game_state.is_player_turn = static_cast<bool>(stoi(lines[j + 1]));
    return is;
}