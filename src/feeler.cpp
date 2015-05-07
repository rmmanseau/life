#include "../headers/feeler.h"

void Feeler::feelSurroundings(const DirVecMap& dirs)
{
    _surroundings.clear();
    forEachDir(dirs, [&](Direction dir){
        char posSym = home()->grid.charAt(pos() + dirs.at(dir));
        _surroundings.insert(CharDirPair(posSym, dir));
    });
}

int Feeler::numberOfSurrounding(Vec2 pos, const DirVecMap& dirs, char condition)
{
    int things = 0;
    forEachDir(dirs, [&](Direction dir){
        if (home()->grid.charAt(pos + dirs.at(dir)) == condition)
            ++things;
    });
    return things;
}

int Feeler::numberOfSurrounding(const DirVecMap& dirs, char condition)
{
    return numberOfSurrounding(pos(), dirs, condition);
}

bool Feeler::surroundingsContain(const DirVecMap& dirs, const CharArr& conditions)
{
    for (int i = 0; i < conditions.size(); i++)
    {
        if (numberOfSurrounding(dirs, conditions[i]) != 0)
            return true;
    }
    return false;
}