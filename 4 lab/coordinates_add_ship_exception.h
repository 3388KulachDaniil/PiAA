#pragma once

#include <stdexcept>

using namespace std;


class CoordinatesAddShipException : public logic_error{
    public:
        CoordinatesAddShipException(): logic_error("Ship cant place near or in other ship"){}
};
