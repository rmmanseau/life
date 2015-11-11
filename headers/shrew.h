#ifndef SHREW_H
#define SHREW_H

// #include "enums.h"
// #include "vec2.h"
// #include "helpers.h"
// #include "terrarium.h"

#include "liver.h"
#include "mover.h"
#include "seer.h"
#include "eater.h"
#include "spawner.h"

class Shrew : public Liver, public Mover, public Seer, public Eater, public Spawner
{
    Direction _currentDir;
    int _whimsy;
    int _directionChance;
    bool _spinning;
    int _spinningClockwise;
    int _preyCount;
    bool _justWalked;
    int _lifespan;

    LifePtr spawn(const Directions dirs, Direction dir);

public:
    Shrew(Terrarium& home, Vec2 pos, char sym = '#');

    void act(LifeArr& newBirths, const DirVecMap& directions);

    inline virtual Terrarium* home() { return _home; }
    inline virtual Vec2 pos()            { return _pos; }
    inline virtual void setPos(Vec2 pos) { _pos = pos; }
    inline virtual int  energy()              { return _energy; }
    inline virtual void setEnergy(int energy) { _energy = energy; }
    inline virtual char sym() { return _sym; }
};

class BabyShrew : public Liver, public Mover, public Spawner
{
    Direction _currentDir;
    int _spinningClockwise;

    LifePtr spawn();

public:
    BabyShrew(Terrarium& home, Vec2 pos);

    void act(LifeArr& newBirths, const DirVecMap& directions);

    inline virtual Terrarium* home() { return _home; }
    inline virtual Vec2 pos()            { return _pos; }
    inline virtual void setPos(Vec2 pos) { _pos = pos; }
    inline virtual void setEnergy(int energy) { _energy = energy; }
    inline virtual char sym() { return _sym; }
};

#endif // SHREW_H
