#include "cell.h"


Cell::Cell(): is_empty{true}, state{Cell::ViewState::HIDDEN}, segment{nullptr} {}

bool Cell::isEmpty(){
    return this->is_empty;
}

void Cell::setSegment(Segment& segment){
    this->segment = &segment;
    this->is_empty = false;
}

Segment& Cell::getSegment(){
    if (isEmpty())
        throw invalid_argument("This cell is empty!");
    return *this->segment;
}

void Cell::setState(Cell::ViewState state){
    this->state = state;
}

Cell::ViewState Cell::getState(){
    return this->state;
}

ostream& operator<<(ostream& stream, const Cell& cell){
    switch (cell.state){
        case Cell::ViewState::HIDDEN:
            stream<<"?";
            break;
        case Cell::ViewState::SHIP:
             stream<<"x";
            break;
        default:
            stream<<"□";
    }
    return stream;
}