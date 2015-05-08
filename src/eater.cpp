#include "../headers/eater.h"

Eater::Eater(CharArr canEat, int energyFromFood)
    : _energyFromFood(energyFromFood)
    , _canEat(canEat)
{}

void Eater::eat(const DirVecMap& dirs, Direction dir)
{
    Vec2 foodPos = pos() + dirs.at(dir);
    if (home()->checkIfValid(foodPos, _canEat))
    {
        home()->grid.removeChar(foodPos);
        setEnergy(energy() + _energyFromFood);
    }
}