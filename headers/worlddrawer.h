#ifndef WORLDDRAWER_H
#define WORLDDRAWER_H

#include <map>
#include <string>
#include <ncurses.h>

#include "imports.h"
#include "enums.h"
#include "vec2.h"

class WorldDrawer
{
    CharVecMap _changes;
    VecIntMap _specifiedColors;

    int gridLength;
    int gridHeight;

    void printSingleColor(char sym, int color, int offX, int offY);
    void printRandomColor(char sym, IntArr colors, int chance, int offX, int offY);
    void printSpecifiedColor(char sym, IntArr colors, int offX, int offY);

public:
    WorldDrawer() {}
    WorldDrawer(const std::string& world, int worldX, int worldY);
    void refresh(const std::string& world, int worldX, int worldY);
    void draw(int offX, int offY);
    void updateColors();

    inline void recordChange(char sym, Vec2 pos) {
        _changes.insert(CharVecPair(sym, pos));
    }
    inline void recordColor(Vec2 pos, int color) {
        _specifiedColors.insert(VecIntPair(pos, color));
    }
};

#endif // WORLDDRAWER_H
