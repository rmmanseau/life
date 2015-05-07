#include "../headers/liver.h"

Liver::Liver(Terrarium& home, Vec2 pos, int energy, char sym)
    : _home(&home)
    , _pos(pos)
    , _energy(energy)
    , _sym(sym)
{}

void Liver::die(int ID, IntArr& deaths)
{
    home()->grid.removeChar(pos());
    deaths.push_back(ID);
}