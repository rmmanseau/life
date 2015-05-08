#include "../headers/worlddrawer.h"

#define _COLOR_BROWN 100
#define _COLOR_DUMB_GRAY 101
#define _COLOR_LIGHT_ROCK 102
#define _COLOR_MIDDLE_ROCK 103
#define _COLOR_DARK_ROCK 104
#define _COLOR_GREEN 105
#define _COLOR_EGG 106
#define _COLOR_LIGHT_BLUE 107
#define _COLOR_RUST 108
#define _COLOR_ROSE_RED 109
#define _COLOR_TULIP_YELLOW 110
#define _COLOR_VIOLET 111

namespace Color {
    const int lightRock = 100;
    const int midRock = 101;
    const int darkRock = 102;
    const int empty = 103;
    const int grass = 104;
    const int redRose = 105;
    const int yellowTulip = 106;
    const int violet = 107;
    const int dumbBug = 108;
    const int smartBug = 109;
    const int egg = 110;
    const int shrew = 111;
}

void createMapColors()
{
    init_color(_COLOR_BROWN, 190, 100, 30);
    init_color(_COLOR_DUMB_GRAY, 850, 840, 820);
    init_color(_COLOR_LIGHT_ROCK, 650, 630, 610);
    init_color(_COLOR_MIDDLE_ROCK, 550, 530, 510);
    init_color(_COLOR_DARK_ROCK, 450, 435, 425);
    init_color(_COLOR_GREEN, 462, 662, 70);
    init_color(_COLOR_EGG, 968, 949, 731);
    init_color(_COLOR_LIGHT_BLUE, 105, 776, 980);
    init_color(_COLOR_RUST, 968, 443, 282);
    init_color(_COLOR_ROSE_RED, 861, 118, 237);
    init_color(_COLOR_TULIP_YELLOW, 937, 860, 100);
    init_color(_COLOR_VIOLET, 776, 513, 921);

    init_pair(Color::empty, _COLOR_BROWN, _COLOR_BROWN);
    init_pair(Color::lightRock, _COLOR_LIGHT_ROCK, _COLOR_BROWN);
    init_pair(Color::midRock, _COLOR_MIDDLE_ROCK, _COLOR_BROWN);
    init_pair(Color::darkRock, _COLOR_DARK_ROCK, _COLOR_BROWN);
    init_pair(Color::grass, _COLOR_GREEN, _COLOR_BROWN);
    init_pair(Color::dumbBug, _COLOR_DUMB_GRAY, _COLOR_BROWN);
    init_pair(Color::smartBug, _COLOR_LIGHT_BLUE, _COLOR_BROWN);
    init_pair(Color::egg, _COLOR_EGG, _COLOR_BROWN);
    init_pair(Color::shrew, _COLOR_RUST, _COLOR_BROWN);
    init_pair(Color::redRose, _COLOR_ROSE_RED, _COLOR_BROWN);
    init_pair(Color::yellowTulip, _COLOR_TULIP_YELLOW, _COLOR_BROWN);
    init_pair(Color::violet, _COLOR_VIOLET, _COLOR_BROWN);
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

    createMapColors();
}

void WorldDrawer::draw() {
    struct Local
    {
        static void singleColorPrint(char sym, int color, const CharVecMap& changes)
        {
            if (changes.count(sym) == 0)
                return;

            attron(COLOR_PAIR(color));

            auto its = changes.equal_range(sym);
            while(its.first != its.second)
            {
                int x = its.first->second.x;
                int y = its.first->second.y;
                mvprintw(y, x, "%c", sym);

                ++its.first;
            }

            attroff(COLOR_PAIR(color));
        }

        static void multiColorPrint(char sym, IntArr colors, const CharVecMap& changes, int chance)
        {
            if (changes.count(sym) == 0)
                return;

            auto its = changes.equal_range(sym);
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
                mvprintw(y, x, "%c", sym);
                attroff(COLOR_PAIR(color));

                ++its.first;
            }
        }
    };

    // start_color();

    Local::singleColorPrint(Sym::empty, Color::empty, _changes);
    Local::multiColorPrint(Sym::rock, {Color::midRock, Color::lightRock, Color::darkRock}, _changes, 2);
    Local::multiColorPrint(Sym::smallPlant, {Color::grass, Color::redRose, Color::yellowTulip, Color::violet}, _changes, 30);
    Local::singleColorPrint(Sym::dumbBug, Color::dumbBug, _changes);
    Local::singleColorPrint(Sym::dumbBugEgg, Color::egg, _changes);
    Local::singleColorPrint(Sym::mSmartBug, Color::smartBug, _changes);
    Local::singleColorPrint(Sym::fSmartBug, Color::smartBug, _changes);
    Local::singleColorPrint(Sym::smartBugEgg, Color::egg, _changes);

    _changes.clear();

    // end_color();
}