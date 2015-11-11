#ifndef SMALLPLANT_H
#define SMALLPLANT_H

#include <vector>
#include <map>

#include "enums.h"
#include "vec2.h"
#include "helpers.h"
#include "terrarium.h"

#include "liver.h"
#include "feeler.h"
#include "spawner.h"

class SmallPlant : public Liver, public Feeler, public Spawner
{
    int _color;

    LifePtr spawn(const Directions dirs, Direction dir);

public:
    SmallPlant(Terrarium& home, Vec2 pos, int color = 0);

    void act(int ID, VecIntMap& newPlants, VecArr& newFlowers, IntArr& newDeaths, const DirVecMap& directions);

    virtual void spawnPlant(const DirVecMap& dirs, Direction dir, VecIntMap& newBirths, int color);

    inline virtual Terrarium* home() { return _home; }
    inline virtual Vec2 pos() { return _pos; }
    inline virtual void setEnergy(int energy) { _energy = energy; }

    inline char sym() { return _sym; }
    inline int energy() { return _energy; }
    inline int color() { return _color; }
};

#endif // SMALLPLANT_H
