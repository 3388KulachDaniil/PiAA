#pragma once

#include <vector>
#include <bits/stdc++.h>

#include "ship.h"

using namespace std;


class ShipsManager{
    public:
        ShipsManager(vector<unsigned short int> ship_lens);
        ShipsManager(size_t ship_count, vector<unsigned short int> ship_lens);
        size_t size();
        Ship& operator[](size_t index);
        size_t getNewDeadsCount();
    private:
        size_t len;
        vector<Ship> ships;
        vector<size_t> dead_ships;
};