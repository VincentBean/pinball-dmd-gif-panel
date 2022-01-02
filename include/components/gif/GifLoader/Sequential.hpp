#pragma once

#include <stack>
#include <SdFat.h>
#include "Loader.hpp"

class Sequential : public Loader
{
public:
    String loadNextFile();

protected:
    FsFile root;
    FsFile curFile;
    std::stack<String> directories;
    String curDirectory;
};