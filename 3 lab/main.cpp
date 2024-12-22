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

#include <iostream>
#include <fstream>

using namespace std;


int main(){
    try{
        vector<short unsigned int> lens = {1, 2, 3, 4, 4};
        ShipsManager player_manager(5, lens);
        ShipsManager enemy_manager(5, lens);
        PlayingField playground_player(7, 7), playground_enemy(7, 7);
        auto playground_player_ptr = make_shared<PlayingField>(playground_player);
        auto player_manager_ptr = make_shared<ShipsManager>(player_manager);
        // playground_player_ptr->addShip(0, 0, player_manager_ptr->operator[](4), Ship::Orientation::VERTICAL);
        // playground_player_ptr->printFull();
        auto playground_enemy_ptr = make_shared<PlayingField>(playground_enemy);
        auto enemy_manager_ptr = make_shared<ShipsManager>(enemy_manager);
        // playground_enemy_ptr->addShip(0, 0, enemy_manager_ptr->operator[](4), Ship::Orientation::HORIZONTAL);
        // playground_enemy_ptr->printFull();
        Game game(playground_player_ptr, playground_enemy_ptr, player_manager_ptr, enemy_manager_ptr);
        game.start();
        game.load("save.txt");
        game.playerTurn(0, 0, true, 0, 0);
        game.enemyTurn();
        game.playerTurn(1, 0, true, 0, 0);
        game.enemyTurn();
        game.playerTurn(1, 0, true, 0, 0);
        game.enemyTurn();
        game.playerTurn(2, 0);
        game.enemyTurn();
        game.load("save.txt");
        game.playerTurn(2, 0);
        game.enemyTurn();
        game.playerTurn(3, 0);
        game.enemyTurn();
        game.playerTurn(3, 0);
        game.enemyTurn();
        game.save("save2.txt");
        cout<<game.isPlayerWin()<<endl;
        cout<<game.isEnemyWin()<<endl;;
    }
    catch(exception& e){
        cerr<<e.what()<<endl;
        return 1;
    }

    return 0;
}