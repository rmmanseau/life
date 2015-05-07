#include "../headers/mover.h"

Mover::Mover(CharArr canMoveOn)
    : _canMoveOn(canMoveOn)
{}

void Mover::move(const DirVecMap& dirs, Direction dir)
{
    Vec2 newPos = pos() + dirs.at(dir);
    if (home()->checkIfValid(newPos, _canMoveOn))
    {
        home()->grid.moveChar(pos(), newPos);
        setPos(newPos);
    }
}