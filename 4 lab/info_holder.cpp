#include "info_holder.h"


InfoHolder::InfoHolder(shared_ptr<PlayingField> playing_field, shared_ptr<ShipsManager> ships_manager, size_t x, size_t y):
    playing_field(playing_field), ships_manager(ships_manager), x(x), y(y) {}

InfoHolder::~InfoHolder(){}