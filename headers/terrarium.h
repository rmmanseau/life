#ifndef TERRARIUM_H
#define TERRARIUM_H

#include <string>
#include <map>
#include <vector>

#include "enums.h"
#include "grid.h"

struct Terrarium
{
    Grid grid;

    Terrarium(std::string map);

    bool checkIfValid(Vec2 pos, std::vector<char> conditions);
};

#endif // TERRARIUM_H
