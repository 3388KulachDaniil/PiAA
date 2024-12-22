#include "segment.h"
#include "ships_manager.h"
#include "ship.h"
#include "playing_field.h"
#include "info_holder.h"
#include "skills_manager.h"
#include "out_of_field_exception.h"
#include "skills/scanner_creator.h"
#include "md5.h"
#include "game_state.h"
#include "game.h"
#include "game_view/playing_field_view.h"
#include "game_view/skills_manager_view.h"
#include "game_manager.h"
#include "game_input/game_control.h"
#include "game_view/game_view.h"
#include "game_view/playing_field_view.h"
#include "game_view/skills_manager_view.h"
#include "game_view/annotation.h"
#include "game_view/ship_view_symbols.h"
#include "game_view/skills_view_symbols.h"

#include <iostream>
#include <fstream>

using namespace std;


int main(){
    try{
        vector<short unsigned int> lens = {1};
        ShipsManager player_manager(lens), enemy_manager(lens);
        PlayingField playground_player(1, 1), playground_enemy(1, 1);
        auto playground_player_ptr = make_shared<PlayingField>(playground_player);
        auto player_manager_ptr = make_shared<ShipsManager>(player_manager);
        auto playground_enemy_ptr = make_shared<PlayingField>(playground_enemy);
        auto enemy_manager_ptr = make_shared<ShipsManager>(enemy_manager);
        Game game(playground_player_ptr, playground_enemy_ptr, player_manager_ptr, enemy_manager_ptr);
        GameManager<GameView<PlayingFieldView<ShipViewSymbols>, SkillsManagerView<SkillsViewSymbols>, Annotation>, GameControl> game_manager(game, "contol.txt");
        game_manager.play();
    }
    catch(exception& e){
        cerr<<e.what()<<endl;
        return 1;
    }

    return 0;
}