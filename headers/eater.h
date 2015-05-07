#ifndef EATER_H
#define EATER_H

#include <map>
#include <vector>

#include "terrarium.h"
#include "enums.h"

class Eater
{
private:
    int _energyFromFood;

protected:
    CharArr _canEat;

public:
    Eater(CharArr canEat, int energyFromFood);

    virtual void eat(const DirVecMap& dirs, Direction dir);

    virtual Terrarium* home() = 0;
    virtual Vec2 pos() = 0;
    virtual void setPos(Vec2 pos) = 0;
    virtual int energy() = 0;
    virtual void setEnergy(int energy) = 0;
};

#endif // EATER_H
