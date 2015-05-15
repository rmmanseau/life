#include <cstdlib>
#include <ctime>
#include <ncurses.h>
#include <time.h>
#include <sstream>

#include "../headers/enums.h"
#include "../headers/vec2.h"
#include "../headers/helpers.h"
#include "../headers/terrarium.h"
#include "../headers/imports.h"
#include "../headers/worlddrawer.h"
#include "../headers/statdrawer.h"
#include "../headers/dumbbug.h"
#include "../headers/smartbug.h"
#include "../headers/shrew.h"
#include "../headers/smallplant.h"
#include "../headers/flower.h"
// SIMPLEMODE
///=== Settings ================================================================
float msFrameSpeed = 15;

int winOffX = 4;
int winOffY = 2;
int statOffX = 2;
int statOffY = 0;
///=============================================================================

template <typename T>
void mergeDeaths(std::vector<T>& living, IntArr& newDeaths)
{
    while (!newDeaths.empty())
    {
        int ID = newDeaths.back();
        newDeaths.pop_back();
        living.erase(living.begin() + ID);
    }
    newDeaths.clear();
}

template <typename T>
int mergeBirths(Terrarium& t, std::vector<T>& living, VecArr& newBirths)
{
    int newBirthAmount = newBirths.size();
    while (!newBirths.empty())
    {
        Vec2 birthPlace = newBirths.back();
        newBirths.pop_back();
        T newBorn(t, birthPlace);
        t.grid.addChar(birthPlace, newBorn.sym());
        living.push_back(newBorn);
    }
    newBirths.clear();
    return newBirthAmount;
}

void printWorldNames(StrStrMap& worlds)
{
    std::cout << "Available worlds include:\n";

    for (auto it = worlds.begin(); it != worlds.end(); ++it)
    {
        std::cout << "  " << it->first << '\n';
    }
}

int main(int argc, char* argv[])
{

    StrStrMap worlds;
    importMaps(worlds);

    if (argc == 1)
    {
        std::cout << "Can not start without a world!\n";
        printWorldNames(worlds);

        exit(1);
    }

    // World Selection

    std::string worldName;
    std::stringstream convert2(argv[1]);
    convert2 >> worldName;

    if (worlds.count(worldName) == 0)
    {
        std::cout << "No such world!\n";
        printWorldNames(worlds);

        exit(1);
    }

    std::string world = worlds.at(worldName);
    
    // Speed Selection (Optional)
    if (argc > 2)
    {
        std::stringstream convert1(argv[2]);
        convert1 >> msFrameSpeed;
    }

    srand(time(0));

    // Create Directions
    DirVecMap directions;
    directions[n]  = Vec2( 0, -1);
    directions[ne] = Vec2( 1, -1);
    directions[e]  = Vec2( 1,  0);
    directions[se] = Vec2( 1,  1);
    directions[s]  = Vec2( 0,  1);
    directions[sw] = Vec2(-1,  1);
    directions[w]  = Vec2(-1,  0);
    directions[nw] = Vec2(-1, -1);

    // Create Terrarium
    Terrarium t(world);

    // ncurses stuff
    initscr();
    raw();
    keypad(stdscr, true);
    noecho();
    timeout(0);
    curs_set(0);
    start_color();

    WorldDrawer picasso(t.grid.map, t.grid.x, t.grid.y);
    t.grid.assignDrawer(picasso);
    picasso.draw(winOffX, winOffY);

    StatDrawer tukey;
    Stats stats;
    statOffX += winOffX + t.grid.x;
    statOffY += winOffY;

    std::vector<DumbBug> dumbBugs;
    std::vector<DumbBugEgg> dumbBugEggs;
    std::vector<SmartBug> smartBugs;
    std::vector<SmartBugEgg> smartBugEggs;
    std::vector<Shrew> shrews;
    std::vector<BabyShrew> babyShrews;
    std::vector<SmallPlant> smallPlants;
    std::vector<Flower> flowers;

    for (int y = 0; y < t.grid.y; ++y) {
        for (int x = 0; x < t.grid.x; ++x) {
            Vec2 pos(x, y);
            char sym = t.grid.charAt(pos);

            switch (sym)
            {
                case Sym::dumbBug:
                    dumbBugs.push_back(DumbBug(t, pos));
                    break;

                case Sym::dumbBugEgg:
                    dumbBugEggs.push_back(DumbBugEgg(t, pos));
                    break;

                case Sym::mSmartBug:
                    smartBugs.push_back(SmartBug(t, pos, Sym::mSmartBug));
                    break;

                case Sym::fSmartBug:
                    smartBugs.push_back(SmartBug(t, pos, Sym::fSmartBug));
                    break;

                case Sym::smartBugEgg:
                    smartBugEggs.push_back(SmartBugEgg(t, pos));
                    break;

                case Sym::mShrew:
                    shrews.push_back(Shrew(t, pos, Sym::mShrew));
                    break;

                case Sym::fShrew:
                    shrews.push_back(Shrew(t, pos, Sym::fShrew));
                    break;

                case Sym::bShrew:
                    babyShrews.push_back(BabyShrew(t, pos));
                    break;

                case Sym::smallPlant:
                    smallPlants.push_back(SmallPlant(t, pos));
                    break;

                case Sym::flower:
                    flowers.push_back(Flower(t, pos));
                    break;
            }
        }
    }

    stats.totalDumbBugs = dumbBugs.size();
    stats.totalSmartBugs = smartBugs.size();
    stats.totalSmallPlants = smallPlants.size();
    stats.totalShrews = shrews.size();
    
    clock_t cycleTime;
    float longestCycle = 0;
    int totalCycles = 0;

    bool keepWinOpen = true;
    bool paused      = false;
    while (keepWinOpen) {

        cycleTime = clock();

        // 'q' to quit
        int in = getch();
        if (in == 'q') {
            keepWinOpen = false;
        }
        else if (in == ' ') {
            paused = !paused;
        }
        else if (in == 'r') {
            picasso.updateColors();
        }
        else if (in == 'R') {
            picasso.refresh(t.grid.map, t.grid.x, t.grid.y);
        }
        else if (in == KEY_SRIGHT)
        {
            if (msFrameSpeed > 95)
                msFrameSpeed = 100;
            else
                msFrameSpeed += 5;
        }
        else if (in == KEY_SLEFT)
        {
            if (msFrameSpeed < 5)
                msFrameSpeed = 0.1;
            else
                msFrameSpeed -= 5;
        }
        else if (in == KEY_RIGHT)
        {
            if (msFrameSpeed > 99.5)
                msFrameSpeed = 100;
            else
                msFrameSpeed += 0.5;
        }
        else if (in == KEY_LEFT)
        {
            if (msFrameSpeed < 0.5)
                msFrameSpeed = 0.1;
            else
                msFrameSpeed -= 0.5;
        }

        if (paused)
        {
            stats.msFrameSpeed = msFrameSpeed;
            picasso.draw(winOffX, winOffY);
            tukey.draw(stats, statOffX, statOffY);
            napms(100);
        }
        else
        {

/*==============================================================================            
                SIM LOOP
==============================================================================*/

//=== Action Loops =============================================================

            VecArr newBirths;
            IntArr newDeaths;

            // Dumb Bug
            if (dumbBugs.size())
            {
                forEach(dumbBugs, [&](DumbBug& l, int id){
                    l.act(id, newBirths, newDeaths, directions);
                });

                mergeDeaths(dumbBugs, newDeaths);
                mergeBirths(t, dumbBugEggs, newBirths);
            }

            // Dumb Bug Egg
            if (dumbBugEggs.size())
            {
                forEach(dumbBugEggs, [&](DumbBugEgg& l, int id){
                    l.act(id, newBirths, newDeaths);
                });

                mergeDeaths(dumbBugEggs, newDeaths);
                stats.totalDumbBugs +=
                    mergeBirths(t, dumbBugs, newBirths);
            }

            // Smart Bug
            if (smartBugs.size())
            {
                forEach(smartBugs, [&](SmartBug& l, int id){
                    l.act(id, newBirths, newDeaths, directions);
                });

                mergeDeaths(smartBugs, newDeaths);
                mergeBirths(t, smartBugEggs, newBirths);
            }

            // Smart Bug Egg
            if (smartBugEggs.size())
            {
                forEach(smartBugEggs, [&](SmartBugEgg& l, int id){
                    l.act(id, newBirths, newDeaths);
                });

                mergeDeaths(smartBugEggs, newDeaths);
                stats.totalSmartBugs +=
                    mergeBirths(t, smartBugs, newBirths);
            }
            
            // Shrew
            if (shrews.size())
            {
                forEach(shrews, [&](Shrew& l, int id){
                    l.act(id, newBirths, newDeaths, directions);
                });

                mergeDeaths(shrews, newDeaths);
                mergeBirths(t, babyShrews, newBirths);
            }

            // Baby Shrew
            if (babyShrews.size())
            {
                forEach(babyShrews, [&](BabyShrew& l, int id){
                    l.act(id, newBirths, newDeaths, directions);
                });

                mergeDeaths(babyShrews, newDeaths);
                stats.totalShrews +=
                    mergeBirths(t, shrews, newBirths);
            }

            // Small Plant
            if (smallPlants.size())
            {
                VecIntMap newPlants;
                VecArr    newFlowers;

                forEach(smallPlants, [&](SmallPlant& l, int id){
                    l.act(id, newPlants, newFlowers, newDeaths, directions);
                });

                mergeDeaths(smallPlants, newDeaths);

                stats.totalSmallPlants += newPlants.size();
                for (auto it = newPlants.begin(); it != newPlants.end(); ++it)
                {
                    Vec2 birthPlace = it->first;
                    int birthColor;
                    if (rand() % 150)
                        birthColor = it->second;
                    else
                        birthColor = rand() % 4;

                    SmallPlant newBorn(t, birthPlace, birthColor);
                    
                    t.grid.addChar(birthPlace, newBorn.sym());
                    picasso.recordColor(birthPlace, newBorn.color());
                    
                    smallPlants.push_back(newBorn);
                }
                newPlants.clear();

                mergeBirths(t, flowers, newFlowers);
            }

            // Flower
            if (flowers.size())
            {
                forEach(flowers, [&](Flower& l, int id){
                    l.act(id, newDeaths, directions);
                });

                mergeDeaths(flowers, newDeaths);
            }

//=== Cycle Speed Calculations =================================================
            
            float msCycleTime = (float)((clock() - cycleTime)/100);
            float msDelay = msFrameSpeed - msCycleTime;

            if (msDelay > 0)
                napms(msDelay);

            picasso.draw(winOffX, winOffY);

            if (totalCycles > 20 && longestCycle < msCycleTime)
                longestCycle = msCycleTime;

            stats.msFrameSpeed = msFrameSpeed;
            stats.msCycleSpeed = msCycleTime;
            stats.msCycleDelay = msDelay;
            stats.msLongestCycle = longestCycle;

            stats.totalCycles = totalCycles;

            stats.currentDumbBugs = dumbBugs.size();
            stats.currentSmallPlants = smallPlants.size();
            stats.currentSmartBugs = smartBugs.size();
            stats.currentShrews = shrews.size();

            stats.totalLife = stats.totalDumbBugs
                              + stats.totalSmallPlants
                              + stats.totalSmartBugs
                              + stats.totalShrews;
            
            stats.currentLife = dumbBugs.size()
                                + smartBugs.size()
                                + smallPlants.size()
                                + shrews.size();
            
            ++totalCycles;

            tukey.draw(stats, statOffX, statOffY);
        }
    }
    refresh();
    endwin();
}