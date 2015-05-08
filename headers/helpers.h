#ifndef HELPERS_H
#define HELPERS_H

#include <cstdlib>
#include <vector>
#include <map>

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

#endif // HELPERS_H
