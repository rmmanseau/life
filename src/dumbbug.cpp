#include "../headers/dumbbug.h"

struct C
{
    struct Bug
    {
        static const int energyInitBase = 100;      // 100
        static const int energyInitVar  = 50;       // 50
        static const int energyFull = 350;          // 300
        static const int energyAfterSpawn = 50;     // 100
        static const int energyFromEating = 20;     // 30
        static const int energyToSpawn = 350;       // 300

        static const int sluggishness = 3;          // 2
    };

    struct Egg
    {
        static const int energyInitBase = 1;    // 1
        static const int energyInitVar = 20;    // 20
        static const int energyAfterSpawn = 0;  // 0
        static const int energyToHatch = 100;   // 100
    };
};

DumbBug::DumbBug(Terrarium& home, Vec2 pos)
    : Liver(home, pos, C::Bug::energyInitBase, C::Bug::energyInitVar, Sym::dumbBug)
    , Mover({Sym::empty, Sym::smallPlant, Sym::flower})
    , Eater({Sym::smallPlant}, C::Bug::energyFromEating)
    , Spawner({Sym::empty}, C::Bug::energyAfterSpawn)
{}

void DumbBug::act(int ID, VecArr& newBirths, IntArr& newDeaths, const DirVecMap& directions)
{
    --_energy;
    
    // Die
    if (_energy <= 0 || wasKilled())
    {
        die(ID, newDeaths);
    }
    // Spawn
    else if (_energy >= C::Bug::energyToSpawn)
    {
        Direction spawnDir = randomElementOfIndex(_canSpawnOn, _feelable);
        spawn(directions, spawnDir, newBirths);
    }
    // Eat
    else if (surroundingsContain(directions, _canEat) && _energy <= C::Bug::energyFull)
    {
        feelSurroundings(directions);
        Direction foodDir = randomElementOfIndex(_canEat, _feelable);
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
    : Liver(home, pos, C::Egg::energyInitBase, C::Egg::energyInitVar, Sym::dumbBugEgg)
    , Spawner({Sym::dumbBugEgg}, C::Egg::energyAfterSpawn)
{}

void DumbBugEgg::act(int ID, VecArr& newBirths, IntArr& newDeaths)
{
    ++_energy;

    // Die
    if (_energy <= 0 || wasKilled())
    {
        die(ID, newDeaths);
    }
    else if (_energy >= C::Egg::energyToHatch)
    {
        spawn(newBirths);
        die(ID, newDeaths);
    }

}