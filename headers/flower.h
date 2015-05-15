#ifndef FLOWER_H
#define FLOWER_H

#include "liver.h"
#include "feeler.h"

struct Flower : public Liver , public Feeler
{
    Flower(Terrarium& home, Vec2 pos);

    void act(int ID, IntArr& newDeaths, const DirVecMap& directions);

    inline virtual Terrarium* home() { return _home; }
    inline virtual Vec2 pos() { return _pos; }

    inline char sym() { return _sym; }
};

#endif // FLOWER_H
