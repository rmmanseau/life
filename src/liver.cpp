#include "../headers/liver.h"

Liver::Liver(Terrarium& home, Vec2 pos, int energyBase, int energyVar, char sym)
    : _home(&home)
    , _pos(pos)
    , _sym(sym)
    , _alive(true)
{
    _energy = energyBase;
    if (energyVar > 0) {
        _energy += rand() % energyVar;
    }
}

void Liver::die()
{
    home()->grid.removeChar(pos());
    _alive = false;
}

bool Liver::dead() {
    return !_alive;
}

bool Liver::wasKilled()
{
    return home()->grid.charAt(pos()) == Sym::empty;
}