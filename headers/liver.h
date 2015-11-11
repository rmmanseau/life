#ifndef LIVER_H
#define LIVER_H

#include <vector>

#include "terrarium.h"
#include "enums.h"

class Liver
{
protected:
    Terrarium* _home;
    Vec2 _pos;
    int _energy;
    char _sym;
    bool _alive;

public:
    Liver(Terrarium& home, Vec2 pos, int energyBase, int energyVar, char sym);

    virtual void die();
    virtual bool wasKilled();
    virtual bool dead();

    virtual Terrarium* home() = 0;
    virtual Vec2 pos() = 0;
};

#endif // LIVER_H
