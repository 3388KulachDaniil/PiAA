#pragma once

#include "segment.h"
#include "ship.h"
#include "cell.h"
#include "coordinates_add_ship_exception.h"
#include "out_of_field_attack_exception.h"
#include "out_of_field_exception.h"

#include <cstring>
#include <memory>


class IAttack;


class PlayingField{
    public:
        PlayingField(size_t width, size_t height);
        PlayingField(const PlayingField& playing_field);
        PlayingField& operator=(const PlayingField& playing_field);
        PlayingField(PlayingField&& playing_field);
        PlayingField& operator=(PlayingField&& playing_field);
        ~PlayingField();
        void addShip(size_t x, size_t y, Ship& ship, Ship::Orientation orientation);
        void setDefaultAttack();
        void setAttackType(shared_ptr<IAttack> attack_creator);
        void base_attack(size_t x, size_t y, unsigned short int damage=1);
        void attack(size_t x, size_t y, unsigned short int damage=1);
        void printFull();
        size_t getWidth();
        size_t getHeight();
        Cell& getCell(size_t x, size_t y);
        bool isAllShipsDead();

        friend ostream& operator<<(ostream& stream, const PlayingField& playing_field);
    private:
        size_t width;
        size_t height;
        Cell** field;

        shared_ptr<IAttack> attack_type;
        bool is_cell_alone(long int x, long int y);
};