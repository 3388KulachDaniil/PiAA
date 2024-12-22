#include "ships_manager.h"

ShipsManager::ShipsManager(vector<unsigned short int> ship_lens){
    this->len = ship_lens.size();
    for (auto ship_len: ship_lens)
        this->ships.push_back(Ship(ship_len));
}

ShipsManager::ShipsManager(size_t ship_count, vector<unsigned short int> ship_lens){
    if (ship_lens.size() < ship_count)
        throw invalid_argument("size of ship_lens must be less or equal ship_count");
    this->len = ship_count;
    for (size_t i = 0; i < ship_count; ++i)
        this->ships.push_back(Ship(ship_lens[i]));
}

size_t ShipsManager::size(){
    return this->len;
}

Ship& ShipsManager::operator[](size_t index){
    return this->ships.at(index);
}

size_t ShipsManager::getNewDeadsCount(){
    size_t k = 0;
    bool alive;
    for (size_t i = 0; i < this->len; ++i){
        alive = false;
        if (count(this->dead_ships.begin(), this->dead_ships.end(), i) == 0){
            for (size_t j = 0; j < this->ships[i].size(); ++j){
                if (this->ships[i][j].getState() != Segment::State::DESTROYED){
                    alive = true;
                    break;
                }
            }
            if (!alive){
                k++;
                this->dead_ships.push_back(i);
            }
        }
    }
    return k;
}
