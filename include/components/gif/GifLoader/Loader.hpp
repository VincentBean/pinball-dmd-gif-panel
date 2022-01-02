#pragma once

#include <Arduino.h>
#include <Globals.h>

class Loader
{
public:
    virtual String loadNextFile() { return ""; };

protected:
    Loader(){};
};