#ifndef GRID_H
#define GRID_H

#include <string>
#include <map>

#include "vec2.h"
#include "enums.h"

struct Grid
{
    std::string map;
    int x;
    int y;

    Grid();
    Grid(std::string s);

    inline char charAt(Vec2 pos)
    {
        return map[pos.y * x + pos.x];
    }
    inline void addChar(Vec2 pos, char c)
    {
        map[pos.y * x + pos.x] = c;
    }
    inline void removeChar(Vec2 pos)
    {
        map[pos.y * x + pos.x] = Sym::empty;
    }
    inline void moveChar(Vec2 from, Vec2 to)
    {
        addChar(to, charAt(from));
        removeChar(from);
    }
};

#endif // GRID_H
