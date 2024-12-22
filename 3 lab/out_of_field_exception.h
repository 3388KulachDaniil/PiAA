#pragma once

#include <stdexcept>

using namespace std;


class OutOfFieldException : public logic_error{
    public:
        OutOfFieldException():logic_error("x must be from range [0, width - 1] and y must be from range range [0, height - 1]"){}
};
