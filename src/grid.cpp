#include "../headers/grid.h"

#include "ncurses.h"

int findGridX(const std::string &s)
{    
    int x = 1;
    for (int i = 0; s[i] != '\n'; i++)
        x++;
    
    return x;
}
int findGridY(const std::string &s, int x)
{
    int y = s.length() / x;
    return y;
}

Grid::Grid() {};

Grid::Grid(std::string s)
    : map(s)
    , x(findGridX(map))
    , y(findGridY(map, x))
{}