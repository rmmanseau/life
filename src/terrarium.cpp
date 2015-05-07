#include "../headers/terrarium.h"

Terrarium::Terrarium(std::string map)
{
    grid = Grid(map);
}

bool Terrarium::checkIfValid(Vec2 pos, std::vector<char> conditions)
{
    for (int i = 0; i < conditions.size(); i++) {
        if (grid.charAt(pos) == conditions[i]) 
            return true;
    }
    return false;
}