#ifndef SEER_H
#define SEER_H

#include <map>
#include <vector>

#include "feeler.h"
#include "terrarium.h"
#include "enums.h"
#include "helpers.h"

class Seer : public Feeler
{
private:
    int _seeDistance;

    char lookInDirection(const DirVecMap& dirs, Direction dir);

protected:
    CharArr _canSeeThrough;
    CharDirMap _seeable;

public:
    Seer(int seeDistance, CharArr _canSeeThrough);

    virtual void viewSurroundings(const DirVecMap& dirs);
    virtual int numberOfSeeable(const CharArr& conditions);
    virtual bool sightContains(const CharArr& conditions);
};

#endif // SEER_H
