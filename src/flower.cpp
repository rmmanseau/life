#include "../headers/flower.h"

Flower::Flower(Terrarium& home, Vec2 pos)
    : Liver(home, pos, 0, 0, Sym::flower)
{}

void Flower::act(int ID, IntArr& newDeaths, const DirVecMap& directions)
{
    if (wasKilled() || numberOfSurrounding(directions, {Sym::dumbBug, Sym::flower, Sym::rock}) > 4)
    {
        die(ID, newDeaths);
    }
    else if (numberOfSurrounding(directions, {Sym::smallPlant}) == 0)
    {
        die(ID, newDeaths);
    }
}