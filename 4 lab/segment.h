#pragma once

#include <iostream>

using namespace std;


class Segment{
    public:
        enum State{
            UNDAMAGED = 2,
            DAMAGED = 1,
            DESTROYED = 0
        };
        Segment();
        State getState();
        void setState(State state);
        void changeStateDelta(int delta);

        friend ostream& operator<<(ostream& stream, const Segment& state);
        friend ostream& operator<<(ostream& stream, const State& state);
    private:
        State state;
};