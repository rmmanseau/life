#ifndef FEELER_H
#define FEELER_H

#include <map>
#include <vector>

#include "terrarium.h"
#include "enums.h"
#include "helpers.h"

class Feeler
{
protected:
    CharDirMap _feelable;

public:
    virtual void feelSurroundings(const DirVecMap& dirs);
    virtual int numberOfSurrounding(Vec2 pos, const DirVecMap& dirs, const CharArr& condition);
    virtual int numberOfSurrounding(const DirVecMap& dirs, const CharArr& conditions);
    virtual bool surroundingsContain(const DirVecMap& dirs, const CharArr& conditions);

    virtual Terrarium* home() = 0;
    virtual Vec2 pos() = 0;
};

#endif // FEELER_H
