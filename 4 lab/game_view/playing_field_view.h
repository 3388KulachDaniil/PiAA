#pragma once

#include "playing_field.h"
#include "i_field_view.h"

using namespace std;


template<typename EnumT>
class PlayingFieldView: public IFieldView{
    public:
        void printAlien(PlayingField playing_field, ostream& stream=cout){
            stream<<static_cast<char>(EnumT::X_DELIMITER)<<static_cast<char>(EnumT::X_DELIMITER);
            for (size_t x = 0; x < playing_field.getWidth(); ++x)
                stream<<this->colorize(x, "\033[91m")<<static_cast<char>(EnumT::X_DELIMITER);
            stream<<static_cast<char>(EnumT::Y_DELIMITER);
            for (size_t y = 0; y < playing_field.getHeight(); ++y){
                stream<<this->colorize(y, "\033[91m")<<static_cast<char>(EnumT::X_DELIMITER);
                for (size_t x = 0; x < playing_field.getWidth(); ++x){
                    switch ( playing_field.getCell(x, y).getState()){
                        case Cell::ViewState::SHIP:
                            switch (playing_field.getCell(x, y).getSegment().getState()){
                                case Segment::State::UNDAMAGED:
                                    stream<<static_cast<char>(EnumT::SHIP_UNDAMAGED);
                                    break;
                                case Segment::State::DAMAGED:
                                    stream<<static_cast<char>(EnumT::SHIP_DAMAGED);
                                    break;
                                case Segment::State::DESTROYED:
                                    stream<<static_cast<char>(EnumT::SHIP_DESTROYED);
                                    break;
                            }                        
                            break;
                        
                        case Cell::ViewState::HIDDEN:
                            stream<<static_cast<char>(EnumT::HIDDEN);
                            break;
                        
                        case Cell::ViewState::EMPTY:
                            stream<<static_cast<char>(EnumT::EMPTY);
                            break;

                        default:
                            break;
                    }
                    stream<<static_cast<char>(EnumT::X_DELIMITER);
                }
                stream<<static_cast<char>(EnumT::Y_DELIMITER);
            }
        }
        void printOwn(PlayingField playing_field, ostream& stream=cout){
            stream<<static_cast<char>(EnumT::X_DELIMITER)<<static_cast<char>(EnumT::X_DELIMITER);
            for (size_t x = 0; x < playing_field.getWidth(); ++x)
                stream<<this->colorize(x, "\033[32m")<<static_cast<char>(EnumT::X_DELIMITER);
            stream<<static_cast<char>(EnumT::Y_DELIMITER);
            for (size_t y = 0; y < playing_field.getHeight(); ++y){
                stream<<this->colorize(y, "\033[32m")<<static_cast<char>(EnumT::X_DELIMITER);
                for (size_t x = 0; x < playing_field.getWidth(); ++x){
                    if (playing_field.getCell(x, y).isEmpty()){
                        stream<<static_cast<char>(EnumT::EMPTY);
                    }
                    else{
                        switch (playing_field.getCell(x, y).getSegment().getState()){
                            case Segment::State::UNDAMAGED:
                                stream<<static_cast<char>(EnumT::SHIP_UNDAMAGED);
                                break;
                            case Segment::State::DAMAGED:
                                stream<<static_cast<char>(EnumT::SHIP_DAMAGED);
                                break;
                            case Segment::State::DESTROYED:
                                stream<<static_cast<char>(EnumT::SHIP_DESTROYED);
                                break;
                        }
                    }
                    stream<<static_cast<char>(EnumT::X_DELIMITER);
                }
                stream<<static_cast<char>(EnumT::Y_DELIMITER);
            }
        }
    private:
        string colorize(char c, string color){
            return color + to_string(c) + "\033[39m";
        }
};