#include "../headers/liver.h"

Liver::Liver(Terrarium& home, Vec2 pos, int energyBase, int energyVar, char sym)
    : _home(&home)
    , _pos(pos)
    , _sym(sym)
{
    _energy = energyBase;
    if (energyVar > 0) {
        _energy += rand() % energyVar;
    }
}

void Liver::die(int ID, IntArr& deaths)
{
    home()->grid.removeChar(pos());
    deaths.push_back(ID);
}

bool Liver::wasKilled()
{
    return home()->grid.charAt(pos()) == Sym::empty;
}