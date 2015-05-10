#ifndef GRID_H
#define GRID_H

#include <string>
#include <map>

#include "vec2.h"
#include "enums.h"
#include "worlddrawer.h"

class Grid
{
    WorldDrawer* drawer;
    inline void informDrawer(char c, Vec2 pos)
    {
        drawer->recordChange(c, pos);
    }

public:
    std::string map;
    int x;
    int y;

    Grid();
    Grid(std::string s);

    inline void assignDrawer(WorldDrawer& in_drawer)
    {
        drawer = &in_drawer;
    }

    inline char charAt(Vec2 pos)
    {
        return map[pos.y * x + pos.x];
    }
    inline void addChar(Vec2 pos, char c)
    {
        map[pos.y * x + pos.x] = c;
        
        #if !SIMPLEMODE
            informDrawer(c, pos);
        #endif
    }
    inline void removeChar(Vec2 pos)
    {
        addChar(pos, Sym::empty);
    }
    inline void moveChar(char sym, Vec2 from, Vec2 to)
    {
        addChar(to, sym);
        removeChar(from);
    }
};

#endif // GRID_H
