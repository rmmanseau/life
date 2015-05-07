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
    auto it = container.begin();
    int randIndex = rand() % container.size();
    return container[randIndex];
}

template <typename T1, typename T2>
T2 randomElementOfIndex(std::vector<T1> indices, const std::multimap<T1, T2>& container)
{
    T1 index = randomElement(indices);

    auto its = container.equal_range(index);
    auto size = std::distance(its.first, its.second);
    auto randIndex = std::rand() % size;
    std::advance(its.first, randIndex);
    return its.first->second;
}

template <typename T1, typename T2>
T2 randomElementOfIndex(T1 index, const std::multimap<T1, T2>& container)
{
    auto its = container.equal_range(index);
    auto size = std::distance(its.first, its.second);
    auto randIndex = std::rand() % size;
    std::advance(its.first, randIndex);
    return its.first->second;
}

inline Direction randomDirection()
{
    return (Direction)(rand() % 8);
}

#endif // HELPERS_H
