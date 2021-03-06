#ifndef HELPERS_H
#define HELPERS_H

#include <cstdlib>
#include <vector>
#include <map>

template <typename T, typename F>
void forEach(T& container, F action)
{
        for (int i = 0; i < container.size(); ++i)
        {
            action(container[i], i);
        }
}

template <typename F>
void forEachDir(const DirVecMap& dirs, F action)
{
    for (int i = 0; i < dirs.size(); ++i)
    {
        action((Direction)i);
    }
}

template <typename T1, typename T2>
T2 randomElement(const std::map<T1, T2>& container)
{
    auto it = container.begin();
    int randIndex = rand() % container.size();
    std::advance(it, randIndex);
    return it.second;
}

template <typename T1, typename T2>
T2 randomElement(const std::multimap<T1, T2>& container)
{    
    auto it = container.begin();
    int randIndex = rand() % container.size();
    std::advance(it, randIndex);
    return it.second;
}

template <typename T>
T randomElement(const std::vector<T>& container)
{ 
    int randIndex = rand() % container.size();
    return container[randIndex];
}


template <typename T1, typename T2>
T2 randomElementOfIndex(T1 index, const std::multimap<T1, T2>& container)
{
    auto its = container.equal_range(index);
    int size = std::distance(its.first, its.second);
    int randIndex;
    if (size > 0)
        randIndex = std::rand() % size;
    else
        randIndex = 0;
    std::advance(its.first, randIndex);
    return its.first->second;
}

template <typename T1, typename T2>
T2 randomElementOfIndex(std::vector<T1> indices, const std::multimap<T1, T2>& container)
{
    T1 index = randomElement(indices);

    return randomElementOfIndex(index, container);
}

inline Direction randomDirection()
{
    return (Direction)(rand() % 8);
}

template <typename T>
bool containerContains(std::vector<T> conditions, T test)
{
    for (int i = 0; i < conditions.size(); i++) {
        if (conditions.at(i) == test)
            return true;
    }
    return false;
}

inline Direction clockwiseOf(Direction dir)
{
    int newDir = (Direction)((int)dir + 1);
    if (newDir > 7)
        newDir = 0;
    return (Direction)newDir;
}

inline Direction counterClockwiseOf(Direction dir)
{
    int newDir = (Direction)((int)dir - 1);
    if (newDir < 0)
        newDir = 7;
    return (Direction)newDir;
}

#endif // HELPERS_H
