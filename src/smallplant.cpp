#include "../headers/smallplant.h"
#include <ncurses.h>


struct C
{
    static const int energyInitBase = 10;
    static const int energyInitVar = 0;
    static const int energyToGrow = 50;
    static const int energyAfterGrow = 10;

    static const int neighborsToBlockGrow = 4;
    static const int neighborsToSkipAct = 7;

    static const int flowerChance = 100;
};

SmallPlant::SmallPlant(Terrarium& home, Vec2 pos, int color)
    : Liver(home, pos, C::energyInitBase, C::energyInitVar, Sym::smallPlant)
    , Spawner({Sym::empty}, C::energyAfterGrow)
    , _color(color)
{}

void SmallPlant::spawnPlant(const DirVecMap& dirs, Direction dir, VecIntMap& newBirths, int color)
{
    Vec2 spawnPos = _pos + dirs.at(dir);
    if (_home->checkIfValid(spawnPos, _canSpawnOn))
    {
        newBirths.insert(VecIntPair(spawnPos, _color));
        _energy = C::energyAfterGrow;
    }
}

void SmallPlant::act(int ID, VecIntMap& newPlants, VecArr& newFlowers, IntArr& newDeaths, const DirVecMap& directions)
{
    int surroundingSmallPlants = numberOfSurrounding(directions, {Sym::smallPlant});
    if (surroundingSmallPlants == C::neighborsToSkipAct)
        return;

    if (surroundingSmallPlants <= 0)
        _energy += 3;
    else if (surroundingSmallPlants <= 3)
        _energy += 2;
    else if (surroundingSmallPlants <= 5)
        _energy += 1;
    else if (surroundingSmallPlants <= 7)
        _energy += 0;
    else if (surroundingSmallPlants <= 8)
        _energy += -2;

    if (_energy <= 0 || wasKilled())
    {
        die(ID, newDeaths);
    }
    else if (_energy >= C::energyToGrow)
    {
        Direction newDir = randomDirection();
        Vec2 newPos = _pos + directions.at(newDir);
        if (numberOfSurrounding(newPos, directions, {Sym::smallPlant}) <= C::neighborsToBlockGrow)
            if (!(rand() % C::flowerChance))
                spawn(directions, newDir, newFlowers);
            else
                spawnPlant(directions, newDir, newPlants, _color);
    }
}