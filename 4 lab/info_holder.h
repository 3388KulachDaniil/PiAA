#pragma once

#include "playing_field.h"
#include "ships_manager.h"
#include <memory>

class InfoHolder{
    public:
        InfoHolder(shared_ptr<PlayingField> playing_field, shared_ptr<ShipsManager> ships_manager, size_t x=0, size_t y=0);
        ~InfoHolder();
        shared_ptr<PlayingField> playing_field;
        shared_ptr<ShipsManager> ships_manager;
        size_t x;
        size_t y;
};