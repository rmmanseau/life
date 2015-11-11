#ifndef ENUMS_H
#define ENUMS_H

#include <map>
#include <vector>
#include <memory>

#include "liver.h"
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
typedef std::multimap<char, Vec2>       CharVecMap;
typedef std::pair<char, Direction>      CharDirPair;
typedef std::pair<char, Vec2>           CharVecPair;
typedef std::map<Vec2, int>             VecIntMap;
typedef std::pair<Vec2, int>            VecIntPair;
typedef std::map<Direction, Vec2>       DirVecMap;

typedef std::unique_ptr<Liver>          LifePtr;
typedef std::vector<LifePtr>            LifeArr;

enum Sym {
    rock        = '@',
    empty       = ' ',
    dumbBug     = 'o',
    dumbBugEgg  = 'e',
    mSmartBug   = 'X',
    fSmartBug   = 'x',
    smartBugEgg = 'a',
    fShrew      = 'S',
    mShrew      = '8',
    bShrew      = 's',
    fFox        = 'Z',
    mFox        = 'M',
    bFox        = 'm',
    smallPlant  = ',',
    flower      = 'v'
};

#endif // ENUMS_H