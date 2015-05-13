#ifndef DUMBBUG_H
#define DUMBBUG_H

#include <vector>
#include <map>

#include "enums.h"
#include "vec2.h"
#include "helpers.h"
#include "terrarium.h"

#include "liver.h"
#include "mover.h"
#include "feeler.h"
#include "eater.h"
#include "spawner.h"

struct DumbBug : public Liver, public Mover, public Feeler, public Eater, public Spawner
{
    DumbBug(Terrarium& home, Vec2 pos);

    void act(int ID, VecArr& newBirths, IntArr& newDeaths, const DirVecMap& directions);

    inline virtual Terrarium* home() { return _home; }
    inline virtual Vec2 pos()            { return _pos; }
    inline virtual void setPos(Vec2 pos) { _pos = pos; }
    inline virtual int  energy()              { return _energy; }
    inline virtual void setEnergy(int energy) { _energy = energy; }
    inline virtual char sym() { return _sym; }
};



struct DumbBugEgg : public Liver, public Spawner
{
    DumbBugEgg(Terrarium& home, Vec2 pos);

    void act(int ID, VecArr& newBirths, IntArr& newDeaths);

    inline virtual Terrarium* home() { return _home; }
    inline virtual Vec2 pos() { return _pos; }
    inline virtual void setEnergy(int energy) { _energy = energy; }
    
    inline char sym() { return _sym; }
    inline char energy() { return _energy; }
};

#endif // DUMBBUG_H
