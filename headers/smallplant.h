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

struct SmallPlant : public Liver, public Feeler, public Spawner
{
    SmallPlant(Terrarium& home, Vec2 pos);

    void act(int ID, VecArr& newPlants, VecArr& newFlowers, IntArr& newDeaths, const DirVecMap& directions);

    inline virtual Terrarium* home() { return _home; }
    inline virtual Vec2 pos() { return _pos; }
    inline virtual void setEnergy(int energy) { _energy = energy; }

    inline char sym() { return _sym; }
    inline int energy() { return _energy; }
};

#endif // SMALLPLANT_H
