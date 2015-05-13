#include <cstdlib>
#include <ctime>
#include <ncurses.h>
#include <time.h>
#include <sstream>

#include "../headers/enums.h"
#include "../headers/vec2.h"
#include "../headers/helpers.h"
#include "../headers/worlds.h"
#include "../headers/terrarium.h"
#include "../headers/worlddrawer.h"
#include "../headers/statdrawer.h"
#include "../headers/dumbbug.h"
#include "../headers/smartbug.h"
#include "../headers/shrew.h"
#include "../headers/smallplant.h"
#include "../headers/flower.h"

///=== Settings ================================================================
#define SIMPLEMODE false

float msFrameSpeed = 0.1;
std::string map = World::test;

int winOffX = 4;
int winOffY = 2;
int statOffX = 2;
int statOffY = 0;
///=============================================================================

/**
 *  This lambda and template heavy act iteration system is
 *  very cumbersome, and I can see it getting in the way
 *  if I add much more complexity. It would probably be worth it
 *  to just remove these functions and iterate through each
 *  vector individually in main.
 *  
 *  I could be wrong, but I also think it's contributing to slow ass
 *  compile times. That would be good to check.
 *  
 *  Might also have to do with the mass amount of files. Looking
 *  into a build system might be a good idea but for unknown
 *  reasons I feel the need to reject them. If a non build system
 *  approach exists, take that.
 *  
 *  ALSO, look into loading things from text files. Would make
 *  trying new colors, maps, and life constants waaaaaaaaaay
 *  easier.
 */

template <typename T>
void mergeDeaths(std::vector<T>& living, IntArr& newDeaths)
{
    while (!newDeaths.empty())
    {
        int ID = newDeaths.back();
        newDeaths.pop_back();
        living.erase(living.begin() + ID);
    }
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
    return newBirthAmount;
}

template <typename P, typename C, typename F>
void letAct(Terrarium& t, std::vector<P>& parents, std::vector<C>& children, F act)
{
    if (parents.size() == 0) return;

    VecArr newBirths;
    IntArr newDeaths;

    for (int i = 0; i < parents.size(); ++i)
    {
        act(i, parents.at(i), newBirths, newDeaths);
    }

    mergeDeaths(parents, newDeaths);
    mergeBirths(t, children, newBirths);
}

template <typename P, typename C, typename F>
void letAct(Terrarium& t, int& counter, std::vector<P>& parents, std::vector<C>& children, F act)
{
    if (parents.size() == 0) return;

    VecArr newBirths;
    IntArr newDeaths;

    for (int i = 0; i < parents.size(); ++i)
    {
        act(i, parents.at(i), newBirths, newDeaths);
    }

    mergeDeaths(parents, newDeaths);
    counter += mergeBirths(t, children, newBirths);
}

template <typename P, typename C, typename C1, typename F>
void letAct(Terrarium& t, int& counter, std::vector<P>& parents, std::vector<C>& children1, std::vector<C1>& children2, F act)
{
    if (parents.size() == 0) return;

    VecArr newBirths1;
    VecArr newBirths2;
    IntArr newDeaths;

    for (int i = 0; i < parents.size(); ++i)
    {
        act(i, parents.at(i), newBirths1, newBirths2, newDeaths);
    }

    mergeDeaths(parents, newDeaths);

    counter += mergeBirths(t, children1, newBirths1);
    counter += mergeBirths(t, children2, newBirths2);
}

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        std::string mapName;
        std::stringstream convert2(argv[1]);
        convert2 >> mapName;

        if (mapName == "valley")
            map = World::valley;
        else if (mapName == "dumbValley")
            map = World::dumbValley;
        else if (mapName == "middleRock")
            map = World::middleRock;
        else if (mapName == "dumbMiddleRock")
            map = World::dumbMiddleRock;
        else if (mapName == "smartMiddleRock")
            map = World::smartMiddleRock;
        else if (mapName == "seed")
            map = World::seed;
        else if (mapName == "test")
            map = World::test;
        else if (mapName == "test1")
            map = World::test1;
        else if (mapName == "seedBig")
            map = World::seedBig;
        else if (mapName == "big")
            map = World::big;
        else if (mapName == "dumbBig")
            map = World::dumbBig;
        else if (mapName == "smartBig")
            map = World::smartBig;
        
        if (argc > 2)
        {
            std::stringstream convert1(argv[2]);
            convert1 >> msFrameSpeed;
        }
    }

    srand(time(0));

    DirVecMap directions;
    directions[n]  = Vec2( 0, -1);
    directions[ne] = Vec2( 1, -1);
    directions[e]  = Vec2( 1,  0);
    directions[se] = Vec2( 1,  1);
    directions[s]  = Vec2( 0,  1);
    directions[sw] = Vec2(-1,  1);
    directions[w]  = Vec2(-1,  0);
    directions[nw] = Vec2(-1, -1);

    Terrarium t(map);

    // ncurses stuff
    initscr();
    raw();
    keypad(stdscr, true);
    noecho();
    timeout(0);
    curs_set(0);

    #if !SIMPLEMODE
        start_color();
        WorldDrawer picasso(map, t.grid.x, t.grid.y);
        t.grid.assignDrawer(picasso);
        picasso.draw(winOffX, winOffY);

        StatDrawer tukey;
        Stats stats;
        statOffX += winOffX + t.grid.x;
        statOffY += winOffY;
    #endif

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
        // mvprintw(t.grid.y + 5, 0, "WE IN");
        int in = getch();
        if (in == 'q') {
            keepWinOpen = false;
        }
        else if (in == ' ') {
            paused = !paused;
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
            napms(100);
        }
        else
        {

/*==============================================================================            
                SIM LOOP
==============================================================================*/

            // Action Loops

            // Dumb Bug
            letAct(t, dumbBugs, dumbBugEggs, 
                   [&directions](int i, DumbBug& actor, VecArr& newBirths, IntArr& newDeaths){
                actor.act(i, newBirths, newDeaths, directions);
            });

            // Dumb Bug Egg
            letAct(t, stats.totalDumbBugs, dumbBugEggs, dumbBugs, 
                   [](int i, DumbBugEgg& actor, VecArr& newBirths, IntArr& newDeaths){
                actor.act(i, newBirths, newDeaths);
            });
            
            // Small Plant
            letAct(t, stats.totalSmallPlants, smallPlants, smallPlants, flowers,
                   [&directions](int i, SmallPlant& actor, VecArr& newPlants, VecArr& newFlowers, IntArr& newDeaths){
                actor.act(i, newPlants, newFlowers, newDeaths, directions);
            });
            
            // Smart Bug
            letAct(t, smartBugs, smartBugEggs, 
                   [&directions](int i, SmartBug& actor, VecArr& newBirths, IntArr& newDeaths){
                actor.act(i, newBirths, newDeaths, directions);
            });

            // Smart Bug Egg
            letAct(t, stats.totalSmartBugs, smartBugEggs, smartBugs, 
                   [](int i, SmartBugEgg& actor, VecArr& newBirths, IntArr& newDeaths){
                actor.act(i, newBirths, newDeaths);
            });

            // Shrew
            letAct(t, shrews, babyShrews, 
                   [&directions](int i, Shrew& actor, VecArr& newBirths, IntArr& newDeaths){
                actor.act(i, newBirths, newDeaths, directions);
            });

            // Baby Shrew
            letAct(t, stats.totalShrews, babyShrews, shrews, 
                   [&directions](int i, BabyShrew& actor, VecArr& newBirths, IntArr& newDeaths){
                actor.act(i, newBirths, newDeaths, directions);
            });

            float msCycleTime = (float)((clock() - cycleTime)/100);
            float msDelay = msFrameSpeed - msCycleTime;

            if (msDelay > 0)
                napms(msDelay);

            #if SIMPLEMODE
                mvprintw(0, 0, "%s", t.grid.map.c_str());
            #else
                picasso.draw(winOffX, winOffY);
            #endif

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
            
            #if !SIMPLEMODE
                tukey.draw(stats, statOffX, statOffY);
            #endif
        }
    }
    refresh();
    endwin();
}