#include "../headers/seer.h"

Seer::Seer(int seeDistance, CharArr canSeeThrough)
    : _seeDistance(seeDistance)
    , _canSeeThrough(canSeeThrough)
{}

char Seer::lookInDirection(const DirVecMap& dirs, Direction dir)
{
    char sym;
    for (int r = 1; r <= _seeDistance; ++r)
    {
        sym = home()->grid.charAt(pos() + ((dirs.at(dir)) * (r)));
        
        if (!containerContains(_canSeeThrough, sym))
            return sym;
    }
    return sym;
}

void Seer::viewSurroundings(const DirVecMap& dirs)
{
    _seeable.clear();

    for (int i = 0; i < dirs.size(); i++)
    {
        Direction dir = (Direction)i;
        char sym = lookInDirection(dirs, dir);
        _seeable.insert(CharDirPair(sym, dir));
    }
}

int Seer::numberOfSeeable(const CharArr& conditions)
{
    int things = 0;
    for (int i = 0; i < conditions.size(); i++)
    {
        things += _seeable.count(conditions.at(i));
    }
    return things;
}
bool Seer::sightContains(const CharArr& conditions)
{
    for (int i = 0; i < conditions.size(); i++)
    {
        if (_seeable.count(conditions.at(i)) != 0)
            return true;
    }
    return false;
}