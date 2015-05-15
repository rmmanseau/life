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

///=== Settings ================================================================
float msFrameSpeed = 15;

int winOffX = 4;
int winOffY = 2;
int statOffX = 2;
int statOffY = 0;

//=== Functions exclusive to main ==============================================

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
    // Loads worlds into a map
    StrStrMap worlds;
    importMaps(worlds);

    // Can't call ./life without a world argument
    if (argc == 1)
    {
        std::cout << "Can not start without a world!\n";
        printWorldNames(worlds);

        exit(1);
    }

    // Grabs world argument
    std::string worldName;
    std::stringstream convert2(argv[1]);
    convert2 >> worldName;

    // Assures that it is a valid name
    if (worlds.count(worldName) == 0)
    {
        std::cout << "No such world!\n";
        printWorldNames(worlds);

        exit(1);
    }

    // Sets world string to selected world
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

    // Create Terrarium with world string
    Terrarium t(world);

    // ncurses stuff
    initscr();
    raw();
    keypad(stdscr, true);
    noecho();
    timeout(0);
    curs_set(0);
    start_color();

    // Create Picasso and Tukey
    WorldDrawer picasso(t.grid.map, t.grid.x, t.grid.y);
    t.grid.assignDrawer(picasso);
    picasso.draw(winOffX, winOffY);

    StatDrawer tukey;
    Stats stats;
    statOffX += winOffX + t.grid.x;
    statOffY += winOffY;

    // Life Vectors
    std::vector<DumbBug> dumbBugs;
    std::vector<DumbBugEgg> dumbBugEggs;
    std::vector<SmartBug> smartBugs;
    std::vector<SmartBugEgg> smartBugEggs;
    std::vector<Shrew> shrews;
    std::vector<BabyShrew> babyShrews;
    std::vector<SmallPlant> smallPlants;
    std::vector<Flower> flowers;

    // Populate Life Vectors with initial state
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

    // Include initial life in the total counters
    stats.totalDumbBugs = dumbBugs.size();
    stats.totalSmartBugs = smartBugs.size();
    stats.totalSmallPlants = smallPlants.size();
    stats.totalShrews = shrews.size();
    
    // Declare cycle Variables
    clock_t cycleTime;
    float longestCycle = 0;
    int totalCycles = 0;

    // Begin Sim Loop
    bool keepWinOpen = true;
    bool paused      = false;
    while (keepWinOpen) {

        // Time at start of cycle
        cycleTime = clock();

        // q to quit
        int in = getch();
        if (in == 'q') {
            keepWinOpen = false;
        }
        // space to pause
        else if (in == ' ') {
            paused = !paused;
        }
        // r to reload color
        else if (in == 'r') {
            picasso.updateColors();
        }
        // R to refresh map
        else if (in == 'R') {
            picasso.refresh(t.grid.map, t.grid.x, t.grid.y);
        }
        // Shift+right to incrament frame duration by 5
        else if (in == KEY_SRIGHT)
        {
            if (msFrameSpeed > 95)
                msFrameSpeed = 100;
            else
                msFrameSpeed += 5;
        }
        // Shift+left to decrament frame duration by 5
        else if (in == KEY_SLEFT)
        {
            if (msFrameSpeed < 5)
                msFrameSpeed = 0.1;
            else
                msFrameSpeed -= 5;
        }
        // right to incrament frame duration by 0.5
        else if (in == KEY_RIGHT)
        {
            if (msFrameSpeed > 99.5)
                msFrameSpeed = 100;
            else
                msFrameSpeed += 0.5;
        }
        // left to decrament frame duration by 0.5
        else if (in == KEY_LEFT)
        {
            if (msFrameSpeed < 0.5)
                msFrameSpeed = 0.1;
            else
                msFrameSpeed -= 0.5;
        }

        // minimal updates while paused
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
    
    /**
     *  Loops through each life array allowing each life form to
     *  take an action.
     *  
     *  Mostly handles management of life arrays, such as births and
     *  deaths. Also takes care of some things that would
     *  be awkward to do from the action call as of right now.
     */

            // Common birth/death arrays usable by most life
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
                
                // Merge Plant Births
                stats.totalSmallPlants += newPlants.size();
                for (auto it = newPlants.begin(); it != newPlants.end(); ++it)
                {
                    Vec2 birthPlace = it->first;
                    int birthColor;
                    
                    // 1/200 chance of new plant being a different color than its parent
                    if (rand() % 200)
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
            
            // Deals with cycle times
            float msCycleTime = (float)((clock() - cycleTime)/100);
            float msDelay = msFrameSpeed - msCycleTime;

            // Delay drawing if cycle happened faster than msFrameSpeed
            if (msDelay > 0)
                napms(msDelay);

            // Picasso draws world changes to display
            picasso.draw(winOffX, winOffY);

            // Deal with stats / counters
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

            // Tukey draws stat changes to display
            tukey.draw(stats, statOffX, statOffY);
        }
    }
    refresh();
    endwin();
}