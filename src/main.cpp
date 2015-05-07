#include <cstdlib>
#include <ctime>
#include <ncurses.h>

#include "../headers/enums.h"
#include "../headers/vec2.h"
#include "../headers/helpers.h"
#include "../headers/worlds.h"
#include "../headers/terrarium.h"
#include "../headers/dumbbug.h"
#include "../headers/smallplant.h"

template <typename T, typename U>
void letAct(Terrarium& t, const DirVecMap& directions, std::vector<T>& lifeForms, std::vector<U>& newBirths)
{
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

template <typename T>
void printInfo(int ID, std::string name, const Terrarium& t, std::vector<T>& lifeForms)
{
    mvprintw(0, t.grid.x + (ID * 30), "%s amount: %i    ", name.c_str(), lifeForms.size());
    for (int i = 0; i < lifeForms.size(); i++)
    {
        mvprintw(i + 1, t.grid.x + (ID * 30), "%s %i energy: %i    ", name.c_str(), i, lifeForms.at(i).energy());
    }
}


int main()
{
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


    // ncurses stuff
    initscr();
    raw();
    keypad(stdscr, true);
    noecho();
    timeout(0);
    curs_set(0);
    start_color();

    Terrarium t(World::test);

    std::vector<DumbBug> dumbBugs;
    std::vector<DumbBugEgg> dumbBugEggs;
    std::vector<SmallPlant> smallPlants;

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
            }
        }
    }

    bool keepWinOpen = true;
    bool paused      = false;
    while (keepWinOpen) {

        // 'q' to quit
        int in = getch();
        if (in == 'q') {
            keepWinOpen = false;
        }
        if (in == ' ') {
            paused = !paused;
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

            letAct(t, directions, dumbBugs, dumbBugEggs);
            letAct(t, directions, dumbBugEggs, dumbBugs);
            letAct(t, directions, smallPlants);

            mvprintw(0, 0, "%s", t.grid.map.c_str());
            
            printInfo(0, "SmallPlant", t, smallPlants);
            printInfo(1, "DumbBug", t, dumbBugs);
            printInfo(2, "DumbBugEgg", t, dumbBugEggs);

            napms(45);
        }


    }
    refresh();
    endwin();
}