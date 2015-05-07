#ifndef ENUMS_H
#define ENUMS_H

#include <map>
#include <vector>
#include "vec2.h"


enum Direction { 
    n,
    ne,
    e,
    se,
    s,
    sw,
    w,
    nw
};

typedef std::vector<char>               CharArr;
typedef std::vector<int>                IntArr;
typedef std::vector<Vec2>               VecArr;
typedef std::multimap<char, Direction>  CharDirMap;
typedef std::pair<char, Direction>      CharDirPair;
typedef std::map<Direction, Vec2>       DirVecMap;

enum Sym {
    rock        = '@',
    empty       = ' ',
    dumbBug     = 'e',
    mSmartBug   = 'X',
    fSmartBug   = 'x',
    dumbBugEgg  = 'o',
    smartBugEgg = 'a',
    fShrew      = '&',
    mShrew      = '8',
    smallPlant  = ','
};

#endif // ENUMS_H