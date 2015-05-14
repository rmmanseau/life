#ifndef IMPORTS_H
#define IMPORTS_H

#include <map>
#include <fstream>

// Color Imports

struct RGB
{
    int r, g, b;
};

typedef std::map<int, RGB> IntRGBMap;
typedef std::pair<int, RGB> IntRGBPair;

IntRGBMap importColors();

#endif // IMPORTS_H
