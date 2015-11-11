#ifndef SPAWNER_H
#define SPAWNER_H

#include <map>
#include <vector>
#include <memory>

#include "terrarium.h"
#include "enums.h"

class Spawner
{
private:
    int _energyAfterSpawn;

protected:
    CharArr _canSpawnOn;

public:
    Spawner(CharArr canSpawnOn, int energyAfterSpawn);

    // virtual bool canSpawn(const DirVecMap& dirs, Direction dir);
    // virtual void spawn(const DirVecMap& dirs, Direction dir, VecArr& newBirths);
    // virtual void spawn(VecArr& newBirths);

    virtual Terrarium* home() = 0;
    virtual Vec2 pos() = 0;
    virtual void setEnergy(int energy) = 0;
};

#endif // SPAWNER_H
