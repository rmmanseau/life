#include "../headers/eater.h"

Eater::Eater(CharArr canEat, int energyFromFood)
    : _canEat(canEat)
    , _energyFromFood(energyFromFood)
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