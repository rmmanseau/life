#include "../headers/shrew.h"

struct C
{
    struct Grown
    {
        static const int energyInitBase   = 60000; //600
        static const int energyInitVar    = 100; //100
        static const int energyFull       = 800; //800
        static const int energyFromEating = 200; //200
        static const int energyToSpawn    = 700; //700
        static const int energyAfterSpawn = 60000; //300

        static const int whimsyInit          = 100; //100
        static const int whimsyFromEating    = 50;  //50
        static const int whimsyFromSpawning  = 100; //100
        // static const int whimsyFromTurning   = 2;   //2
        static const int whimsyThreshold     = 150;  //15

        static const int sightDistance   = 15; //15  
        static const int directionChance = 15; //10  // 1/x chance
        static const int spinChance      = 20; //20
    };

    struct Baby
    {

    };
};

Shrew::Shrew(Terrarium& home, Vec2 pos, char sym)
    : Liver(home, pos, C::Grown::energyInitBase, C::Grown::energyInitVar, sym)
    , Mover({Sym::empty, Sym::smallPlant})
    , Seer(C::Grown::sightDistance, {Sym::empty, Sym::smallPlant})
    , Eater({Sym::dumbBug, Sym::mSmartBug, Sym::fSmartBug}, C::Grown::energyFromEating)
    , Spawner({Sym::empty, Sym::smallPlant}, C::Grown::energyAfterSpawn)
    , _whimsy(C::Grown::whimsyInit)
    , _directionChance(C::Grown::directionChance)
    , _spinning(false)
    , _spinningClockwise(true)
{
    if (_sym == '#')
    {
        rand() % 2 ? _sym = Sym::mShrew : _sym = Sym::fShrew;
    }
    _currentDir = randomDirection();
}

void Shrew::act(int ID, VecArr& newBirths, IntArr& newDeaths, const DirVecMap& directions)
{
    --_energy;

    if (_energy < 600)
        _whimsy -= 2;

    if (_whimsy > C::Grown::whimsyThreshold)
    {
        _spinning = true;
        _directionChance = 2;
    }
    else
    {
        _spinning = false;
        _directionChance = C::Grown::directionChance;
    }

    // Die
    if (_energy < 0 || _home->grid.charAt(_pos) == Sym::empty)
    {
        die(ID, newDeaths);
    }
    // Eat
    else if (_energy < C::Grown::energyFull
             && surroundingsContain(directions, _canEat))
    {
        feelSurroundings(directions);
        Direction foodDir = randomElementOfIndex(_canEat, _feelable);
        eat(directions, foodDir);
        _whimsy += C::Grown::whimsyFromEating;
    }
    // Spawn
    else if (_sym == Sym::fShrew
             && _energy >= C::Grown::energyToSpawn
             && surroundingsContain(directions, {Sym::mShrew}))
    {
        feelSurroundings(directions);
        Direction mateDir = randomElementOfIndex(_canSpawnOn, _feelable);
        spawn(directions, mateDir, newBirths);
        _whimsy += C::Grown::whimsyFromSpawning;
    }
    // View Surroundings
    else
    {
        feelSurroundings(directions);
        viewSurroundings(directions);

        // Walk To Food
        if (_energy <= C::Grown::energyFull
            && sightContains(_canEat))
        {
            _currentDir = randomElementOfIndex(_canEat, _seeable);
            move(directions, _currentDir);
        }
        // Walk To Mate
        else if (_sym == Sym::mShrew
                 && sightContains({Sym::fShrew}))
        {
            _currentDir = randomElementOfIndex({Sym::fShrew}, _seeable);
            move(directions, _currentDir);
        }
        // Change Directions
        else if ((rand() % _directionChance == 0)
                 || canMoveTowards(directions, _currentDir))
        {
            // Spin
            if (_spinning)
            {
                if (rand() % C::Grown::spinChance == 0)
                    _spinningClockwise = !_spinningClockwise;

                if (_spinningClockwise)
                    _currentDir = clockwiseOf(_currentDir);
                else
                    _currentDir = counterClockwiseOf(_currentDir);

                move(directions, _currentDir);
            }
            // Turn
            else
            {
                _currentDir = randomDirection();
            }
        }
        // Move
        else
        {
            move(directions, _currentDir);
        }
    }
}