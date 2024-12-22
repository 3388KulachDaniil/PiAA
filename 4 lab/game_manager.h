#pragma once

#include "game.h"

using namespace std;

template<class GameViewT, class GameControlT>
class GameManager {
    public:
        GameManager(Game& game, string control_filename): game(game), game_view(GameViewT()){
            try{
                game_control = GameControlT();
                game_control.load(control_filename);
            } catch (exception& e) {
                game_view.pringDefaultControl(e.what());
                game_control.setDefault();
            }
        }

        void play(bool new_game=true){
            if (new_game){
                try{
                    inputShips();
                }
                catch(exception& e) {
                    game_view.printShipsInvalid();
                    return;
                }
                
            }
            game_view.printPlayerField(game.getPlayerField());
            game_view.printEnemyField(game.getEnemyField());
            game.start();
            game_view.printHelp(
                this->game_control["load"], this->game_control["save"], this->game_control["attack"],
                this->game_control["print_skills"], this->game_control["help"]
            );
            while (!game.isEnemyWin() && !game.isPlayerWin()){
                char command;
                cin>>command;
                try{
                    this->handleCommand(command);
                } catch (exception& e){
                    cout<<e.what()<<endl;
                    continue;
                }
            }

            if (game.isPlayerWin() && game.isEnemyWin()){
                game_view.printDraw();
                this->play(true);
            }
            if (game.isPlayerWin()){
                game_view.printWin();
                this->makeNewEnemy();
                this->play(false);
            }
            else if (game.isEnemyWin()){
                game_view.printLose();
                this->play(true);
            }
        }

    private:
        Game& game;
        GameViewT game_view;
        GameControlT game_control;

        void makeNewEnemy(){
            vector<unsigned short int> ship_lens;
            ShipsManager ships = game.getEnemyShips();
            for (size_t i = 0; i < ships.size(); ++i){
                ship_lens.push_back(ships[i].size());
            }
            random_device rd;
            default_random_engine engine(rd());
            size_t field_width = game.getEnemyField().getWidth();
            size_t field_height = game.getEnemyField().getHeight();
            PlayingField enemy(field_width, field_height); 
            shared_ptr<ShipsManager> enemy_ships = make_shared<ShipsManager>(ShipsManager(ship_lens));
            shared_ptr<PlayingField> enemy_field = make_shared<PlayingField>(enemy);

            for (size_t i = 0; i < enemy_ships->size(); ++i){
                try{
                    enemy_field->addShip(engine()%field_width, engine()%field_height, enemy_ships->operator[](i), static_cast<Ship::Orientation>(engine() % 2));
                }
                catch (exception& e){
                    continue;
                }
            }

            game.reload_enemy(enemy_field, enemy_ships);
        }

        void handleCommand(char command){
            string command_str = game_control.parseCommand(command);
            if (command_str == "help")
                game_view.printHelp(
                    this->game_control["load"], this->game_control["save"], this->game_control["attack"],
                    this->game_control["print_skills"], this->game_control["help"]
                );
            else if (command_str == "print_skills")
                game_view.printSkills(game.getSkillsManager());
            else if (command_str == "attack"){
                char command;
                bool use_skill = false;
                size_t skill_x = 0, skill_y = 0;
                game_view.printSkillQ(this->game_control["yes"], this->game_control["no"]);
                cin>>command;
                if (command == this->game_control["yes"])
                    use_skill = true;
                if (use_skill){
                    game_view.printSetSkillCoordsQ(this->game_control["yes"], this->game_control["no"]);
                    cin>>command;
                    if (command == this->game_control["yes"]){
                        game_view.printInputSkillCoords();
                        cin>>skill_x>>skill_y;
                    }
                }
                size_t x, y;
                game_view.printInputAttackCoords();
                cin>>x>>y;
                bool skill_result = this->game.playerTurn(x, y, use_skill, skill_x, skill_y);
                if (use_skill)
                    game_view.printSkillResult(skill_result);
                this->game.enemyTurn();
                game_view.printPlayerField(game.getPlayerField());
                game_view.printEnemyField(game.getEnemyField());
            }
            else if (command_str == "load"){
                string filename;
                game_view.printInputFile();
                cin>>filename;
                game.load(filename);
            }
            else if (command_str == "save"){
                string filename;
                game_view.printInputFile();
                cin>>filename;
                game.save(filename);
            }
            else{
                throw logic_error("Неверная команда");
            }
        }

        void inputShips(){
            size_t a, b, c, d, field_width=0, field_height=0;
            string buffer;
            game_view.printInputShips();
            cin>>a>>b>>c>>d;
            game_view.printInputField();
            cin>>field_width>>field_height;
            if (field_width == 0 || field_height == 0)
                throw logic_error("Размер поля не может быть равен нулю!");
            PlayingField playing_field(field_width, field_height);
            PlayingField enemy_playing_field(field_width, field_height);
            vector<unsigned short int> ship_lens;
            for (size_t i = 0; i < a; ++i)
                ship_lens.push_back(1);
            for (size_t i = 0; i < b; ++i)
                ship_lens.push_back(2);
            for (size_t i = 0; i < c; ++i)
                ship_lens.push_back(3);
            for (size_t i = 0; i < d; ++i)
                ship_lens.push_back(4);
            ShipsManager player_ships(ship_lens);
            ShipsManager enemy_shipss(ship_lens);

            shared_ptr<PlayingField> field = make_shared<PlayingField>(playing_field);
            shared_ptr<PlayingField> enemy_field = make_shared<PlayingField>(enemy_playing_field);
            shared_ptr<ShipsManager> ships = make_shared<ShipsManager>(player_ships);
            shared_ptr<ShipsManager> enemy_ships = make_shared<ShipsManager>(enemy_shipss);

            random_device rd;
            default_random_engine engine(rd());

            for (size_t i = 0; i < player_ships.size(); ++i){
                try{
                    field->addShip(engine()%field_width, engine()%field_height, ships->operator[](i), static_cast<Ship::Orientation>(engine() % 2));
                }
                catch (exception& e){
                     game_view.printSkipShip();
                }
                try{
                    enemy_field->addShip(engine()%field_width, engine()%field_height, enemy_ships->operator[](i), static_cast<Ship::Orientation>(engine() % 2));
                }
                catch (exception& e){
                    continue;
                }
            }

            game.reload_game(field, enemy_field, ships, enemy_ships);
        }
};