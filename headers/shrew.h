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
private:
    Direction _currentDir;
    int _whimsy;
    int _directionChance;
    bool _spinning;
    int _spinningClockwise;

public:
    Shrew(Terrarium& home, Vec2 pos, char sym = '#');

    void act(int ID, VecArr& newBirths, IntArr& newDeaths, const DirVecMap& directions);

    inline virtual Terrarium* home() { return _home; }
    inline virtual Vec2 pos()            { return _pos; }
    inline virtual void setPos(Vec2 pos) { _pos = pos; }
    inline virtual int  energy()              { return _energy; }
    inline virtual void setEnergy(int energy) { _energy = energy; }
    inline virtual char sym() { return _sym; }
};

class BabyShrew : public Liver, public Mover, public Spawner
{
private:
    Direction _currentDir;
    int _spinningClockwise;

public:
    BabyShrew(Terrarium& home, Vec2 pos);

    void act(int ID, VecArr& newBirths, IntArr& newDeaths, const DirVecMap& directions);

    inline virtual Terrarium* home() { return _home; }
    inline virtual Vec2 pos()            { return _pos; }
    inline virtual void setPos(Vec2 pos) { _pos = pos; }
    inline virtual void setEnergy(int energy) { _energy = energy; }
    inline virtual char sym() { return _sym; }
};

#endif // SHREW_H
