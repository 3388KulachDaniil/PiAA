#include <exception>

#include "ship.h"

ostream& operator<<(ostream& stream, const Ship& ship){
    cout<<"Данные экземпляра Ship:\n";
    cout<<"\tДлина: "<<ship.len<<endl;
    cout<<"\tОриентация: "<<ship.orientation<<endl;
    cout<<"\tCегменты:\n";
    for (auto segment: ship.segments)
        cout<<"\t\t"<<segment<<endl;
    return stream;
}

ostream& operator<<(ostream& stream, const Ship::Orientation& orientation){
    switch(orientation){
        case Ship::Orientation::HORIZONTAL:
            stream<<"HORIZONTAL";
            break;
        case Ship::Orientation::VERTICAL:
            stream<<"VERTICAL";
            break;
        default:
            stream<<"NONE";
    }
    return stream;
}

Ship::Ship(unsigned short int len): len{len}{
    if (len == 0 || len > 4)
        throw invalid_argument("Ship's len must be in range [1, 4]");
    
    this->orientation = Ship::Orientation::NONE;
    this->segments = vector<Segment>(len, Segment());
}

Ship::Ship(unsigned short int len, Orientation orientation): Ship(len){
    this->orientation = orientation;
}

Ship::Orientation Ship::getOrientation(){
    return this->orientation;
}

void Ship::setOrientation(Orientation orientation){
    this->orientation = orientation;
}

unsigned short int Ship::size(){
    return this->len;
}

Segment& Ship::operator[](size_t index){
    return this->segments.at(index);
}