#include "../headers/worlddrawer.h"
#include <fstream>

#define _COLOR_DIRT 100
#define _COLOR_LIGHT_ROCK 101
#define _COLOR_MIDDLE_ROCK 102
#define _COLOR_DARK_ROCK 103
#define _COLOR_DUMB 104
#define _COLOR_GRASS 105
#define _COLOR_FLOWER_RED 106
#define _COLOR_FLOWER_YELLOW 107
#define _COLOR_FLOWER_VIOLET 108
#define _COLOR_EGG 109
#define _COLOR_M_SMART 110
#define _COLOR_F_SMART 111
#define _COLOR_M_SHREW 112
#define _COLOR_F_SHREW 113
#define _COLOR_B_SHREW 114

namespace Color {
    const int lightRock = 100;
    const int midRock = 101;
    const int darkRock = 102;
    const int empty = 103;
    const int grass = 104;
    const int flowerRed = 105;
    const int flowerYellow = 106;
    const int flowerViolet = 107;
    const int dumbBug = 108;
    const int mSmartBug = 109;
    const int fSmartBug = 110;
    const int egg = 111;
    const int mShrew = 112;
    const int fShrew = 113;
    const int bShrew = 114;
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
    init_pair(Color::grass, _COLOR_GRASS, _COLOR_DIRT);
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

WorldDrawer::WorldDrawer(const std::string& world, int worldX, int worldY)
{
    for (int y = 0; y < worldY; ++y) {
        for (int x = 0; x < worldX; ++x) {
            Vec2 pos(x, y);
            char sym = world[pos.y * worldX + pos.x];

            recordChange(sym, pos);
        }
    }

    updateColors();
}
void WorldDrawer::print(char sym, int color, int offX, int offY)
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

void WorldDrawer::print(char sym, IntArr colors, int chance, int offX, int offY)
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

void WorldDrawer::draw(int offX, int offY) {

    print(Sym::empty, Color::empty, offX, offY);
    print(Sym::rock, {Color::midRock, Color::lightRock, Color::darkRock}, 2, offX, offY);
    print(Sym::smallPlant, Color::grass, offX, offY);
    print(Sym::flower, {Color::flowerRed, Color::flowerYellow, Color::flowerViolet}, 1, offX, offY);
    print(Sym::dumbBug, Color::dumbBug, offX, offY);
    print(Sym::dumbBugEgg, Color::egg, offX, offY);
    print(Sym::mSmartBug, Color::mSmartBug, offX, offY);
    print(Sym::fSmartBug, Color::fSmartBug, offX, offY);
    print(Sym::smartBugEgg, Color::egg, offX, offY);
    print(Sym::mShrew, Color::mShrew, offX, offY);
    print(Sym::fShrew, Color::fShrew, offX, offY);
    print(Sym::bShrew, Color::bShrew, offX, offY);

    _changes.clear();
}
