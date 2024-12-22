#pragma once


#include "playing_field.h"
#include "ships_manager.h"
#include "skills_manager.h"
#include "ships_manager.h"
#include "info_holder.h"
#include <cxxabi.h>
#include <memory>

class Game;


class GameState {
    friend class Game;

    public:
        GameState(shared_ptr<PlayingField> player_field, shared_ptr<PlayingField> enemy_field, shared_ptr<ShipsManager> player_ships, shared_ptr<ShipsManager> enemy_ships);
        friend ostream& operator<<(ostream& os, const GameState& game_state);
        friend istream& operator>>(istream& is, GameState& game_state);

    private:
        shared_ptr<PlayingField> player_field;
        shared_ptr<PlayingField> enemy_field;
        shared_ptr<ShipsManager> player_ships;
        shared_ptr<ShipsManager> enemy_ships;
        InfoHolder info_holder;
        SkillsManager skills_manager;
        bool is_player_turn = true;
        bool is_game_started = false;

        vector<pair<size_t, size_t>> getShipPosition(Ship& ship, PlayingField& field) const;
        string serializeShips(ShipsManager& ships_manager, PlayingField& field) const;
        string serializeField(PlayingField& field) const;
        string serializeSkills(const SkillsManager& skills_manager) const;
        vector<string> split(const string &s, char delim);
        vector<tuple<size_t, Ship::Orientation, vector<tuple<size_t, size_t, Segment::State>>>> readShips(vector<string>& lines, size_t& j);
        void updateFieldWithShips(PlayingField& field, ShipsManager& ships_manager, vector<tuple<size_t, Ship::Orientation, vector<tuple<size_t, size_t, Segment::State>>>>& ships, vector<string>& lines, size_t& j);
};