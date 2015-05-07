#include "../headers/spawner.h"

Spawner::Spawner(CharArr canSpawnOn, int energyAfterSpawn)
    : _canSpawnOn(canSpawnOn)
    , _energyAfterSpawn(energyAfterSpawn)
{}

void Spawner::spawn(const DirVecMap& dirs, Direction dir, VecArr& newBirths)
{
    Vec2 spawnPos = pos() + dirs.at(dir);
    if (home()->checkIfValid(spawnPos, _canSpawnOn))
    {
        newBirths.push_back(spawnPos);
        setEnergy(_energyAfterSpawn);
    }
}

void Spawner::spawn(VecArr& newBirths)
{
    if (home()->checkIfValid(pos(), _canSpawnOn))
    {
        newBirths.push_back(pos());
    }
}