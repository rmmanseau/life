#include "../headers/worlddrawer.h"
#include <fstream>

#define _COLOR_DIRT 100
#define _COLOR_LIGHT_ROCK 101
#define _COLOR_MIDDLE_ROCK 102
#define _COLOR_DARK_ROCK 103
#define _COLOR_DUMB 104
#define _COLOR_GRASS1 105
#define _COLOR_GRASS2 106
#define _COLOR_GRASS3 107
#define _COLOR_GRASS4 108
#define _COLOR_FLOWER_RED 109
#define _COLOR_FLOWER_YELLOW 110
#define _COLOR_FLOWER_VIOLET 111
#define _COLOR_EGG 112
#define _COLOR_M_SMART 113
#define _COLOR_F_SMART 114
#define _COLOR_M_SHREW 115
#define _COLOR_F_SHREW 116
#define _COLOR_B_SHREW 117

namespace Color {
    const int lightRock = 100;
    const int midRock = 101;
    const int darkRock = 102;
    const int empty = 103;
    const int grass1 = 104;
    const int grass2 = 105;
    const int grass3 = 106;
    const int grass4 = 107;
    const int flowerRed = 108;
    const int flowerYellow = 109;
    const int flowerViolet = 110;
    const int dumbBug = 111;
    const int mSmartBug = 112;
    const int fSmartBug = 113;
    const int egg = 114;
    const int mShrew = 115;
    const int fShrew = 116;
    const int bShrew = 117;
}

void WorldDrawer::updateColors()
{
    IntRGBMap importedColors = importColors();

    for (auto it = importedColors.begin(); it != importedColors.end(); ++it)
    {
        init_color(it->first, it->second.r, it->second.g, it->second.b);
    }

    init_pair(Color::empty, _COLOR_DIRT, _COLOR_DIRT);
    init_pair(Color::lightRock, _COLOR_LIGHT_ROCK, _COLOR_DIRT);
    init_pair(Color::midRock, _COLOR_MIDDLE_ROCK, _COLOR_DIRT);
    init_pair(Color::darkRock, _COLOR_DARK_ROCK, _COLOR_DIRT);
    init_pair(Color::grass1, _COLOR_GRASS1, _COLOR_DIRT);
    init_pair(Color::grass2, _COLOR_GRASS2, _COLOR_DIRT);
    init_pair(Color::grass3, _COLOR_GRASS3, _COLOR_DIRT);
    init_pair(Color::grass4, _COLOR_GRASS4, _COLOR_DIRT);
    init_pair(Color::flowerRed, _COLOR_FLOWER_RED, _COLOR_DIRT);
    init_pair(Color::flowerYellow, _COLOR_FLOWER_YELLOW, _COLOR_DIRT);
    init_pair(Color::flowerViolet, _COLOR_FLOWER_VIOLET, _COLOR_DIRT);
    init_pair(Color::dumbBug, _COLOR_DUMB, _COLOR_DIRT);
    init_pair(Color::mSmartBug, _COLOR_M_SMART, _COLOR_DIRT);
    init_pair(Color::fSmartBug, _COLOR_F_SMART, _COLOR_DIRT);
    init_pair(Color::egg, _COLOR_EGG, _COLOR_DIRT);
    init_pair(Color::mShrew, _COLOR_M_SHREW, _COLOR_DIRT);
    init_pair(Color::fShrew, _COLOR_F_SHREW, _COLOR_DIRT);
    init_pair(Color::bShrew, _COLOR_B_SHREW, _COLOR_DIRT);
}

void WorldDrawer::refresh(const std::string& world, int worldX, int worldY)
{
    for (int y = 0; y < worldY; ++y) {
        for (int x = 0; x < worldX; ++x) {
            Vec2 pos(x, y);
            char sym = world[pos.y * worldX + pos.x];

            recordChange(sym, pos);
        }
    }
}

WorldDrawer::WorldDrawer(const std::string& world, int worldX, int worldY)
    : gridLength(worldX)
    , gridHeight(worldY)
{
    updateColors();
    refresh(world, worldX, worldY);
}


void WorldDrawer::printSingleColor(char sym, int color, int offX, int offY)
{
    if (_changes.count(sym) == 0)
        return;

    attron(COLOR_PAIR(color));

    auto its = _changes.equal_range(sym);
    while(its.first != its.second)
    {
        int x = its.first->second.x;
        int y = its.first->second.y;
        mvprintw(y + offY, x + offX, "%c", sym);

        ++its.first;
    }

    attroff(COLOR_PAIR(color));
}

void WorldDrawer::printRandomColor(char sym, IntArr colors, int chance, int offX, int offY)
{
    if (_changes.count(sym) == 0)
        return;

    auto its = _changes.equal_range(sym);
    while (its.first != its.second)
    {
        int color;
        if (rand() % chance == 0)
            color = colors.at((rand() % colors.size()-1) + 1);
        else
            color = colors.at(0);

        int x = its.first->second.x;
        int y = its.first->second.y;
        
        attron(COLOR_PAIR(color));
        mvprintw(y + offY, x + offX, "%c", sym);
        attroff(COLOR_PAIR(color));

        ++its.first;
    }
}

void WorldDrawer::printSpecifiedColor(char sym, IntArr colors, int offX, int offY)
{
    if (_changes.count(sym) == 0)
        return;

    auto its = _changes.equal_range(sym);
    while (its.first != its.second)
    {
        int color;
        int specColor = 0;

        if (_specifiedColors.count(its.first->second) != 0)
            specColor = _specifiedColors.at(its.first->second);

        if (specColor < 0 || specColor > colors.size())
            specColor = 0;
        
        color = colors.at(specColor);

        int x = its.first->second.x;
        int y = its.first->second.y;

        attron(COLOR_PAIR(color));
        mvprintw(y + offY, x + offX, "%c", sym);
        attroff(COLOR_PAIR(color));

        ++its.first;
    }
}

void WorldDrawer::draw(int offX, int offY) {

    printSingleColor(Sym::empty, Color::empty, offX, offY);
     auto rocks = {Color::midRock, Color::lightRock, Color::darkRock};
     printRandomColor(Sym::rock, rocks, 2, offX, offY);
    auto grasses = {Color::grass1, Color::grass2, Color::grass3, Color::grass4};
    printSpecifiedColor(Sym::smallPlant, grasses, offX, offY);
     auto flowers = {Color::flowerRed, Color::flowerYellow, Color::flowerViolet};
     printRandomColor(Sym::flower, flowers, 1, offX, offY);
    printSingleColor(Sym::dumbBug, Color::dumbBug, offX, offY);
     printSingleColor(Sym::dumbBugEgg, Color::egg, offX, offY);
    printSingleColor(Sym::mSmartBug, Color::mSmartBug, offX, offY);
     printSingleColor(Sym::fSmartBug, Color::fSmartBug, offX, offY);
    printSingleColor(Sym::smartBugEgg, Color::egg, offX, offY);
     printSingleColor(Sym::mShrew, Color::mShrew, offX, offY);
    printSingleColor(Sym::fShrew, Color::fShrew, offX, offY);
     printSingleColor(Sym::bShrew, Color::bShrew, offX, offY);

    _changes.clear();
    _specifiedColors.clear();
}

