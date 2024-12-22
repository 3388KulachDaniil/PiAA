#include "segment.h"

ostream& operator<<(ostream& stream, const Segment::State& state){
    switch(state){
        case Segment::State::UNDAMAGED:
            stream<<"UNDAMAGED";
            break;
        case Segment::State::DAMAGED:
            stream<<"DAMAGED";
            break;
        default:
            stream<<"DESTROYED";
    }
    return stream;
}

ostream& operator<<(ostream& stream, const Segment& segment){
    switch(segment.state){
        case Segment::State::UNDAMAGED:
            stream<<"UNDAMAGED";
            break;
        case Segment::State::DAMAGED:
            stream<<"DAMAGED";
            break;
        default:
            stream<<"DESTROYED";
    }
    return stream;
}

Segment::Segment(): state{State::UNDAMAGED} {}

Segment::State Segment::getState(){
    return this->state;
}

void Segment::setState(State state){
    this->state = state;
}

void Segment::changeStateDelta(int delta){
    int new_value = state + delta;
    if (new_value >= Segment::State::UNDAMAGED)
        state = Segment::State::UNDAMAGED;
    else if (new_value <= Segment::State::DESTROYED)
        state = Segment::State::DESTROYED;
    else
        state = Segment::State::DAMAGED;
}
