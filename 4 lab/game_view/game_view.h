#pragma once

#include "playing_field.h"
#include "skills_manager.h"
//#include "annotation.h"

using namespace std;


template<class FieldViewT, class SkillsViewT, class AnnotationT>
class GameView{
    public:
        GameView(): field_view(FieldViewT()), skills_view(SkillsViewT()), annotation(AnnotationT()){};
        void printEnemyField(PlayingField playing_field, ostream& stream=cout){
            stream<<annotation.ENEMY_FIELD;
            field_view.printAlien(playing_field, stream);
        }
        void printPlayerField(PlayingField playing_field, ostream& stream=cout){
            stream<<annotation.PLAYER_FIELD;
            field_view.printOwn(playing_field, stream);
        }
        void printSkills(SkillsManager* skills_manager, ostream& stream=cout){
            skills_view.printSkills(skills_manager, stream);
        }
        void pringDefaultControl(const char* what, ostream& stream=cout){
            stream<<what<<" "<<annotation.DEFAULT_CONTROL;
        }
        void printShipsInvalid(ostream& stream=cout){
            stream<<annotation.SHIPS_INVALID;
        }
        void printDraw(ostream& stream=cout){
            stream<<annotation.DRAW;
        }
        void printWin(ostream& stream=cout){
            stream<<annotation.WIN;
        }
        void printLose(ostream& stream=cout){
            stream<<annotation.LOSE;
        }
        void printSkillQ(char yes, char no,ostream& stream=cout){
            stream<<annotation.USE_SKILL_Q<<yes<<"/"<<no<<":";
        }
        void printSetSkillCoordsQ(char yes, char no, ostream& stream=cout){
            stream<<annotation.SET_COORDS_SKILL_Q<<yes<<"/"<<no<<":";
        }
        void printInputSkillCoords(ostream& stream=cout){
            stream<<annotation.INPUT_SKILL_COORDS;
        }
        void printInputAttackCoords(ostream& stream=cout){
            stream<<annotation.INPUT_ATTACK_COORDS;
        }
        void printSkillResult(bool result, ostream& stream=cout){
            stream<<annotation.SKILL_ANS<<result<<endl;
        }
        void printInputFile(ostream& stream=cout){
            stream<<annotation.INPUT_FILENAME;
        }
        void printInputShips(ostream& stream=cout){
            stream<<annotation.INPUT_SHIPS;
        }
        void printInputField(ostream& stream=cout){
            stream<<annotation.FIELD_INPUT;
        }
        void printSkipShip(ostream& stream=cout){
            stream<<annotation.SKIP_SHIP;
        }
        void printHelp(char load, char save, char attack, char skills, char help, ostream& stream=cout){
            stream<<annotation.HELP_LINES[0]<<endl;
            stream<<annotation.HELP_LINES[1]<<" -- "<<load<<endl;
            stream<<annotation.HELP_LINES[2]<<" -- "<<save<<endl;
            stream<<annotation.HELP_LINES[3]<<" -- "<<attack<<endl;
            stream<<annotation.HELP_LINES[4]<<" -- "<<skills<<endl;
            stream<<annotation.HELP_LINES[5]<<" -- "<<help<<endl;
        }
    private:
        FieldViewT field_view;
        SkillsViewT skills_view;
        AnnotationT annotation;
};