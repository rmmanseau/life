#include "../headers/shrew.h"

struct C
{
    struct Adlt
    {
        static const int energyInitBase   = 1000; //600
        static const int energyInitVar    = 200; //100
        static const int energyFull       = 1400; //800
        static const int energyHunting    = 600;
        static const int energyFromEating = 700; //200
        static const int energyToSpawn    = 1250; //700
        static const int energyAfterSpawn = 700; //300

        static const int whimsyInit          = 100; //100
        static const int whimsyFromEating    = 50;  //50
        static const int whimsyFromSpawning  = 75;  //100
        static const int whimsyThreshold     = 600; //150
        static const int whimsyDrain         = 4;  //4

        static const int sightDistance   = 15; //15  
        static const int directionChance = 15; //15  // 1/x chance
        static const int spinChance      = 20; //20

        static const int seenEnoughPrey = 6; //6

        static const int lifespanBase = 3000; //5000
    };

    struct Baby
    {
        static const int energyInitBase = 40;  //40
        static const int energyInitVar = 10;   //10
        static const int energyToSpawn = 200;  //200
        static const int energyAfterSpawn = 0; //0

        static const int spinChance = 8; // 8
    };
};

Shrew::Shrew(Terrarium& home, Vec2 pos, char sym)
    : Liver(home, pos, C::Adlt::energyInitBase, C::Adlt::energyInitVar, sym)
    , Mover({Sym::empty, Sym::smallPlant, Sym::flower})
    , Seer(C::Adlt::sightDistance, {Sym::empty, Sym::smallPlant, Sym::flower})
    , Eater({Sym::dumbBug, Sym::mSmartBug, Sym::fSmartBug}, C::Adlt::energyFromEating)
    , Spawner({Sym::empty, Sym::smallPlant, Sym::flower}, C::Adlt::energyAfterSpawn)
    , _whimsy(C::Adlt::whimsyInit)
    , _directionChance(C::Adlt::directionChance)
    , _spinning(false)
    , _spinningClockwise(true)
    , _preyCount(0)
    , _justWalked(false)
    , _lifespan(C::Adlt::lifespanBase)
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
    --_lifespan;

    if (_energy < C::Adlt::whimsyThreshold)
        _whimsy -= C::Adlt::whimsyDrain;

    if (_whimsy > C::Adlt::whimsyThreshold)
    {
        _spinning = true;
        _directionChance = 2;
    }
    else
    {
        _spinning = false;
        _directionChance = C::Adlt::directionChance;
    }

    // Die
    if (_energy < 0 || _lifespan < 0 || wasKilled())
    {
        die(ID, newDeaths);
    }
    // Eat
    else if (_preyCount > C::Adlt::seenEnoughPrey
             && _energy < C::Adlt::energyFull
             && surroundingsContain(directions, _canEat))
    {
        feelSurroundings(directions);
        Direction foodDir = randomElementOfIndex(_canEat, _feelable);
        eat(directions, foodDir);
        _whimsy += C::Adlt::whimsyFromEating;
        _preyCount = 0;
    }
    // Spawn
    else if (_sym == Sym::fShrew
             && _energy >= C::Adlt::energyToSpawn
             && surroundingsContain(directions, {Sym::mShrew}))
    {
        feelSurroundings(directions);
        Direction mateDir = randomElementOfIndex(_canSpawnOn, _feelable);
        spawn(directions, mateDir, newBirths);
        _whimsy += C::Adlt::whimsyFromSpawning;
        _preyCount = 0;
    }
    // View Surroundings
    else
    {
        feelSurroundings(directions);
        viewSurroundings(directions);

        // Walk To Food
        if (_energy <= C::Adlt::energyHunting
            && sightContains(_canEat))
        {
            _currentDir = randomElementOfIndex(_canEat, _seeable);
            move(directions, _currentDir);
            ++_preyCount;
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
                if (rand() % C::Adlt::spinChance == 0)
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
            // if (_justWalked)
                move(directions, _currentDir);
            _justWalked = !_justWalked;
        }
    }
}

BabyShrew::BabyShrew(Terrarium& home, Vec2 pos)
    : Liver(home, pos, C::Baby::energyInitBase, C::Baby::energyInitVar, Sym::bShrew)
    , Mover({Sym::empty, Sym::smallPlant, Sym::flower})
    , Spawner({Sym::bShrew}, C::Baby::energyAfterSpawn)
    , _spinningClockwise(false)
{
    _currentDir = randomDirection();   
}

void BabyShrew::act(int ID, VecArr& newBirths, IntArr& newDeaths, const DirVecMap& directions)
{
    ++_energy;

    if (_energy < 0 || wasKilled())
    {
        die(ID, newDeaths);
    }
    else if (_energy > C::Baby::energyToSpawn)
    {
        spawn(newBirths);
    }
    else
    {
        if (rand() % C::Baby::spinChance == 0)
            _spinningClockwise = !_spinningClockwise;

        if (_spinningClockwise)
            _currentDir = clockwiseOf(_currentDir);
        else
            _currentDir = counterClockwiseOf(_currentDir);

        move(directions, _currentDir);
    }
}