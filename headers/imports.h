#ifndef IMPORTS_H
#define IMPORTS_H

#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>

// Color Imports

struct RGB
{
    int r, g, b;
};

typedef std::map<int, RGB> IntRGBMap;
typedef std::pair<int, RGB> IntRGBPair;
IntRGBMap importColors();

typedef std::map<std::string, std::string> StrStrMap;
typedef std::pair<std::string, std::string> StrStrPair;
void importMaps(StrStrMap& worlds);

#endif // IMPORTS_H
