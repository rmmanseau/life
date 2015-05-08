#ifndef WORLDDRAWER_H
#define WORLDDRAWER_H

#include <map>
#include <string>
#include <ncurses.h>

#include "enums.h"
#include "vec2.h"

class WorldDrawer
{
    CharVecMap _changes;

public:
    WorldDrawer() {}
    WorldDrawer(const std::string& world, int worldX, int worldY);
    void draw();

    inline void recordChange(char sym, Vec2 pos) {
        _changes.insert(CharVecPair(sym, pos));
    }
};

#endif // WORLDDRAWER_H
