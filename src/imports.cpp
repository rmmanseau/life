#include "../headers/imports.h"

IntRGBMap importColors()
{
    std::ifstream colorFile;
    colorFile.open("../config/colors.cfg");
    
    std::string tmp;

    IntRGBMap colors;

    float x = 1000/255.f;

    while (getline(colorFile, tmp, ':'))
    {
        int id;
        RGB color;

        colorFile >> id >> color.r >> color.g >> color.b;

        color.r = (int)(x * color.r + 0.5);
        color.g = (int)(x * color.g + 0.5);
        color.b = (int)(x * color.b + 0.5);

        colors.insert(IntRGBPair(id, color));
    }

    colorFile.close();
    
    return colors;
}