#ifndef MOVER_H
#define MOVER_H

#include <map>
#include <vector>

#include "terrarium.h"
#include "enums.h"
#include "helpers.h"

class Mover
{
protected:
    CharArr _canMoveOn;

public:
    Mover(CharArr canWalkOn);

    virtual void move(const DirVecMap& dirs, Direction dir);
    virtual bool canMoveTowards(const DirVecMap& dirs, Direction dir);

    virtual Terrarium* home() = 0;
    virtual Vec2 pos() = 0;
    virtual void setPos(Vec2 pos) = 0;
};

#endif // MOVER_H
