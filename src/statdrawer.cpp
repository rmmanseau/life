#include "../headers/statdrawer.h"

#define _NORM_BACK 200
#define _NORM_TEXT 201

namespace Color {
    const int norm = 200;
}

void createStatColors()
{
    init_color(_NORM_BACK, 0, 0, 0);
    init_color(_NORM_TEXT, 900, 900, 900);

    init_pair(Color::norm, _NORM_TEXT, _NORM_BACK);
}

StatDrawer::StatDrawer()
{
    createStatColors();
}

void StatDrawer::draw(const Stats& stats, int offX, int offY)
{
    attron(COLOR_PAIR(Color::norm));

    mvprintw(offY + 1, offX + 1, " Frame Speed:   "); 
    mvprintw(offY + 2, offX + 1, " Cycle Speed:   "); 
    mvprintw(offY + 3, offX + 1, " Cycle Delay:   "); 
    mvprintw(offY + 4, offX + 1, " Longest Cycle: "); 
    mvprintw(offY + 1, offX + 18, " %i ", stats.msFrameSpeed);
    mvprintw(offY + 2, offX + 18, " %i ", stats.msCycleSpeed);
    mvprintw(offY + 3, offX + 18, " %i ", stats.msCycleDelay);
    mvprintw(offY + 4, offX + 18, " %i ", stats.msLongestCycle);

    mvprintw(offY + 6, offX + 1, " Total Cycles: "); 
    mvprintw(offY + 7, offX + 1, " Total Life:   "); 
    mvprintw(offY + 8, offX + 1, " Current Life: "); 
    mvprintw(offY + 6, offX + 17, " %i ", stats.totalCycles);    
    mvprintw(offY + 7, offX + 17, " %i ", stats.totalLife);
    mvprintw(offY + 8, offX + 17, " %i ", stats.currentLife);

    mvprintw(offY + 10 , offX + 1, " Total DumbBugs:   "); 
    mvprintw(offY + 11 , offX + 1, " Current DumbBugs: "); 
    mvprintw(offY + 10, offX + 21, " %i ", stats.totalDumbBugs);    
    mvprintw(offY + 11, offX + 21, " %i ", stats.currentDumbBugs);

    mvprintw(offY + 13, offX + 1, " Total SmartBugs:   "); 
    mvprintw(offY + 14, offX + 1, " Current SmartBugs: "); 
    mvprintw(offY + 13, offX + 22, " %i ", stats.totalSmartBugs);
    mvprintw(offY + 14, offX + 22, " %i ", stats.currentSmartBugs);

    mvprintw(offY + 16, offX + 1, " Total SmallPlants:   "); 
    mvprintw(offY + 17, offX + 1, " Current SmallPlants: ");
    mvprintw(offY + 16, offX + 24, " %i ", stats.totalSmallPlants);
    mvprintw(offY + 17, offX + 24, " %i ", stats.currentSmallPlants);

    mvprintw(offY + 19, offX + 1, " Total Shrews:   "); 
    mvprintw(offY + 20, offX + 1, " Current Shrews: ");
    mvprintw(offY + 19, offX + 19, " %i ", stats.totalShrews);
    mvprintw(offY + 20, offX + 19, " %i ", stats.currentShrews);
    
    attroff(COLOR_PAIR(Color::norm));
}