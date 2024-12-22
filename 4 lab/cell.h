#pragma once

#include "segment.h"


class Cell{
    public:
        enum ViewState{
            HIDDEN = 0,
            EMPTY = 1,
            SHIP = 2
        };

        Cell();
        bool isEmpty();
        void setSegment(Segment& segment);
        Segment& getSegment();
        void setState(ViewState state);
        ViewState getState();

        friend ostream& operator<<(ostream& stream, const Cell& cell);

    private:
        bool is_empty;
        ViewState state;
        Segment* segment;
};