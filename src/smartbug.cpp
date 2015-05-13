#include "../headers/smartbug.h"

struct C
{
    struct Bug
    {
        static const int energyInitBase   = 50; //300
        static const int energyInitVar    = 50; //300
        static const int energyToSpawn    = 400; //400
        static const int energyAfterSpawn = 50; //100
        static const int energyFromEating = 20;  //20
        static const int energyFull       = 450; //450

        static const int sightDistance = 4;          // 4
        static const int changeDirectionChance = 20; // 20  // 1/x chance
    };

    struct Egg
    {
        static const int energyInitBase = 1;
        static const int energyInitVar = 20;
        static const int energyAfterSpawn = 0;
        static const int energyToHatch = 100;
    };
};

SmartBug::SmartBug(Terrarium& home, Vec2 pos, char sym)
    : Liver(home, pos, C::Bug::energyInitBase, C::Bug::energyInitVar, sym)
    , Mover({Sym::empty, Sym::smallPlant, Sym::flower})
    , Seer(C::Bug::sightDistance, {Sym::empty, Sym::smallPlant, Sym::flower})
    , Eater({Sym::smallPlant, Sym::flower}, C::Bug::energyFromEating)
    , Spawner({Sym::empty, Sym::smallPlant, Sym::flower}, C::Bug::energyAfterSpawn)
{
    if (_sym == '#')
    {
        rand() % 2 ? _sym = Sym::mSmartBug : _sym = Sym::fSmartBug;
    }
    _currentDir = randomDirection();
}

void SmartBug::act(int ID, VecArr& newBirths, IntArr& newDeaths, const DirVecMap& directions)
{
    --_energy;
    
    // Die
    if (_energy <= 0 || wasKilled())
    {
        die(ID, newDeaths);
    }
    // Eat
    else if (_energy <= C::Bug::energyFull
        && surroundingsContain(directions, _canEat))
    {
        feelSurroundings(directions);
        Direction foodDir = randomElementOfIndex(_canEat, _feelable);
        eat(directions, foodDir);
    }
    // Spawn
    else if (_sym == Sym::fSmartBug
             && _energy >= C::Bug::energyToSpawn
             && surroundingsContain(directions, {Sym::mSmartBug}))
    {
        feelSurroundings(directions);
        Direction mateDir = randomElementOfIndex(_canSpawnOn, _feelable);
        spawn(directions, mateDir, newBirths);
    }
    // View Surroundings
    else
    {   
        feelSurroundings(directions);
        viewSurroundings(directions);

        // Walk To Food
        if (_energy <= C::Bug::energyFull
             && sightContains(_canEat))
        {
            _currentDir = randomElementOfIndex(_canEat, _seeable);
            move(directions, _currentDir);
        }
        // Walk To Mate
        else if (_sym == Sym::mSmartBug
                 && sightContains({Sym::fSmartBug}))
        {
            _currentDir = randomElementOfIndex({Sym::fSmartBug}, _seeable);
            move(directions, _currentDir);
        }
        // Change Direction
        else if (canMoveTowards(directions, _currentDir)
                 || (rand() % C::Bug::changeDirectionChance == 0))
        {
            _currentDir = randomDirection();
        }
        // Walk
        else
        {
            move(directions, _currentDir);
        }
    }
}



SmartBugEgg::SmartBugEgg(Terrarium& home, Vec2 pos)
    : Liver(home, pos, C::Egg::energyInitBase, C::Egg::energyInitVar, Sym::smartBugEgg)
    , Spawner({Sym::smartBugEgg}, C::Egg::energyAfterSpawn)
{}

void SmartBugEgg::act(int ID, VecArr& newBirths, IntArr& newDeaths)
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