#pragma once

#include "playing_field.h"


class IFieldView{
    public:
        ~IFieldView()=default;
        virtual void printAlien(PlayingField playing_field, ostream& stream=cout)=0;
        virtual void printOwn(PlayingField playing_field, ostream& stream=cout)=0;
};