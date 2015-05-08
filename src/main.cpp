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
#include "../headers/dumbbug.h"
#include "../headers/smartbug.h"
#include "../headers/smallplant.h"

///=== Settings ================================================================
#define SIMPLEMODE false
float msFrameSpeed = 0.1;
std::string map = World::test;
///=============================================================================

template <typename T, typename U>
void letAct(Terrarium& t, const DirVecMap& directions, std::vector<T>& lifeForms, std::vector<U>& newBirths)
{
    if (lifeForms.size() == 0)
        return;

    VecArr birthVec;
    IntArr deathVec;

    for (int i = 0; i < lifeForms.size(); ++i)
    {
        lifeForms[i].act(i, birthVec, deathVec, directions);
    }

    while (!deathVec.empty())
    {
        int ID = deathVec.back();
        deathVec.pop_back();
        lifeForms.erase(lifeForms.begin() + ID);
    }

    while (!birthVec.empty())
    {
        Vec2 birthPlace = birthVec.back();
        birthVec.pop_back();
        U newBorn(t, birthPlace);
        t.grid.addChar(birthPlace, newBorn.sym());
        newBirths.push_back(newBorn);
    }
}

template <typename T>
void letAct(Terrarium& t, const DirVecMap& directions, std::vector<T>& lifeForms)
{
    letAct(t, directions, lifeForms, lifeForms);
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
        picasso.draw();
    #endif

    std::vector<DumbBug> dumbBugs;
    std::vector<DumbBugEgg> dumbBugEggs;
    std::vector<SmallPlant> smallPlants;
    std::vector<SmartBug> smartBugs;
    std::vector<SmartBugEgg> smartBugEggs;

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

                case Sym::smallPlant:
                    smallPlants.push_back(SmallPlant(t, pos));
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
            }
        }
    }

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
            mvprintw(t.grid.y + 1, 5, "Min Frame Speed: %f      ", msFrameSpeed);
            napms(100);
        }
        else
        {

/*==============================================================================            
                SIM LOOP
==============================================================================*/

            letAct(t, directions, dumbBugs, dumbBugEggs);
            letAct(t, directions, dumbBugEggs, dumbBugs);
            letAct(t, directions, smallPlants);
            letAct(t, directions, smartBugs, smartBugEggs);
            letAct(t, directions, smartBugEggs, smartBugs);

            float msCycleTime = (float)((clock() - cycleTime)/100);

            float msDelay = msFrameSpeed - msCycleTime;
            if (msDelay > 0)
                napms(msDelay);

            #if SIMPLEMODE
                mvprintw(0, 0, "%s", t.grid.map.c_str());
            #else
                picasso.draw();
            #endif

            if ( totalCycles > 20 && longestCycle < msCycleTime)
                longestCycle = msCycleTime;

            mvprintw(t.grid.y, 5, "Delay: %f     Cycle: %f      ", msDelay, msCycleTime);
            mvprintw(t.grid.y + 1, 5, "Min Frame Speed: %f      ", msFrameSpeed);
            mvprintw(t.grid.y + 2, 5, "Longest Cycle: %f     ", longestCycle);

            ++totalCycles;
        }
    }
    refresh();
    endwin();
}