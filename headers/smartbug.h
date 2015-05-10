#ifndef SMARTBUG_H
#define SMARTBUG_H

#include <vector>
#include <map>

#include "enums.h"
#include "vec2.h"
#include "helpers.h"
#include "terrarium.h"

#include "liver.h"
#include "mover.h"
#include "seer.h"
#include "eater.h"
#include "spawner.h"

class SmartBug : public Liver, public Mover, public Seer, public Eater, public Spawner
{
private:
    Direction _currentDir;

public:
    SmartBug(Terrarium& home, Vec2 pos, char sym = '#');

    void act(int ID, VecArr& newBirths, IntArr& newDeaths, const DirVecMap& directions);

    inline virtual Terrarium* home() { return _home; }
    inline virtual Vec2 pos()            { return _pos; }
    inline virtual void setPos(Vec2 pos) { _pos = pos; }
    inline virtual int  energy()              { return _energy; }
    inline virtual void setEnergy(int energy) { _energy = energy; }
    inline virtual char sym() { return _sym; }
};

struct SmartBugEgg : public Liver, public Spawner
{
    SmartBugEgg(Terrarium& home, Vec2 pos);

    void act(int ID, VecArr& newBirths, IntArr& newDeaths, const DirVecMap& directions);

    inline virtual Terrarium* home() { return _home; }
    inline virtual Vec2 pos() { return _pos; }
    inline virtual void setEnergy(int energy) { _energy = energy; }

    inline char sym() { return _sym; }
    inline char energy() { return _energy; }
};

#endif // SMARTBUG_H
