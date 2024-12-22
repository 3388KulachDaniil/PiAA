#pragma once

#include <vector>

#include "segment.h"

using namespace std;


class Ship{
    public:
        enum Orientation{
            NONE = -1,
            VERTICAL = 0,
            HORIZONTAL = 1
        };
        Ship(unsigned short int len);
        Ship(unsigned short int len, Orientation orientation);
        Orientation getOrientation();
        void setOrientation(Orientation orientation);
        unsigned short int size();
        Segment& operator[](size_t index);

        friend ostream& operator<<(ostream& stream, const Ship& ship);
        friend ostream& operator<<(ostream& stream, const Orientation& state);
    private:
        unsigned short int len;
        Orientation orientation;
        vector<Segment> segments;
};