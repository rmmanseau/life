#ifndef STATDRAWER_H
#define STATDRAWER_H

#include <string>
#include <ncurses.h>

struct Stats
{
    int msFrameSpeed = 0;
    int msCycleSpeed = 0;
    int msCycleDelay = 0;
    int msLongestCycle = 0;

    int totalCycles = 0;

    int currentLife = 0;
    int totalLife = 0;

    int currentDumbBugs = 0;
    int totalDumbBugs = 0;

    int currentSmartBugs = 0;
    int totalSmartBugs = 0;

    int currentSmallPlants = 0;
    int totalSmallPlants = 0;
};

class StatDrawer
{
    int messagesDrawn = 0;

    void drawBox(int offX, int offY, int length, int height, int color);

public:
    StatDrawer();

    void draw(const Stats& stats, int offX, int offY);
};

#endif // STATDRAWER_H
