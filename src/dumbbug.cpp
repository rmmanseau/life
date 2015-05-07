#include "../headers/dumbbug.h"

struct C
{
    struct Bug
    {
        static const int energyInitBase = 100;      // 100
        static const int energyInitVar  = 50;       // 50
        static const int energyFull = 300;          // 300
        static const int energyAfterSpawn = 100;    // 100
        static const int energyFromEating = 30;     // 30
        static const int energyToSpawn = 300;       // 300

        static const int sluggishness = 2;          // 2
    };

    struct Egg
    {
        static const int energyInitBase = 1;    // 1
        static const int energyAfterSpawn = 0;  // 0
        static const int energyToHatch = 100;   // 100
    };
};

DumbBug::DumbBug(Terrarium& home, Vec2 pos)
    : Liver(home, pos, C::Bug::energyInitBase, Sym::dumbBug)
    , Mover({Sym::empty, Sym::smallPlant})
    , Eater({Sym::smallPlant}, C::Bug::energyFromEating)
    , Spawner({Sym::empty}, C::Bug::energyAfterSpawn)
{}

void DumbBug::act(int ID, VecArr& newBirths, IntArr& newDeaths, const DirVecMap& directions)
{
    --_energy;
    feelSurroundings(directions);
    
    // Die
    if (_energy <= 0 || _home->grid.charAt(_pos) == Sym::empty)
    {
        die(ID, newDeaths);
    }
    // Spawn
    if (_energy >= C::Bug::energyToSpawn)
    {
        Direction spawnDir = randomElementOfIndex(_canSpawnOn, _surroundings);
        spawn(directions, spawnDir, newBirths);
    }
    // Eat
    else if (surroundingsContain(directions, _canEat) && _energy <= C::Bug::energyFull)
    {
        Direction foodDir = randomElementOfIndex(_canEat, _surroundings);
        eat(directions, foodDir);
    }
    // Move
    else
    {
        if (!(rand() % C::Bug::sluggishness)) 
        {
            Direction walkDir = randomDirection();
            move(directions, walkDir);
        }
    }
}


DumbBugEgg::DumbBugEgg(Terrarium& home, Vec2 pos)
    : Liver(home, pos, C::Egg::energyInitBase, Sym::dumbBugEgg)
    , Spawner({Sym::dumbBugEgg}, C::Egg::energyAfterSpawn)
{}

void DumbBugEgg::act(int ID, VecArr& newBirths, IntArr& newDeaths, const DirVecMap& directions)
{
    ++_energy;

    // Die
    if (_energy <= 0 || _home->grid.charAt(_pos) == Sym::empty)
    {
        die(ID, newDeaths);
    }
    else if (_energy >= C::Egg::energyToHatch)
    {
        spawn(newBirths);
        newDeaths.push_back(ID);
    }

}