#include "../headers/feeler.h"

void Feeler::feelSurroundings(const DirVecMap& dirs)
{
    _feelable.clear();
    forEachDir(dirs, [&](Direction dir){
        char posSym = home()->grid.charAt(pos() + dirs.at(dir));
        _feelable.insert(CharDirPair(posSym, dir));
    });
}

int Feeler::numberOfSurrounding(Vec2 pos, const DirVecMap& dirs, const CharArr& conditions)
{
    int things = 0;
    forEachDir(dirs, [&](Direction dir){
        for (int i = 0; i < conditions.size(); i++)
        {
            if (home()->grid.charAt(pos + dirs.at(dir)) == conditions.at(i))
                ++things;
        }
    });
    return things;
}

int Feeler::numberOfSurrounding(const DirVecMap& dirs, const CharArr& conditions)
{
    return numberOfSurrounding(pos(), dirs, conditions);
}

bool Feeler::surroundingsContain(const DirVecMap& dirs, const CharArr& conditions)
{
    for (int i = 0; i < conditions.size(); i++)
    {
        if (numberOfSurrounding(dirs, conditions) != 0)
            return true;
    }
    return false;
}