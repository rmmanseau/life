#include "../headers/imports.h"

IntRGBMap importColors()
{
    std::ifstream colorFile;
    colorFile.open("../config/colors.dat");
    
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

void importMaps(StrStrMap& worlds)
{
    std::ifstream mapFile;

    char c;
    char prev;

    mapFile.open("../config/worlds.dat");
    while (mapFile.get(c))
    {
        if (c == ':')
        {
            int baseLength = -1;
            int length = 0;
            std::stringstream world_in;
            std::string world;
            std::string title;

            getline(mapFile, title, ':');

            for (int i = 0; i < title.size(); ++i)
            {
                if (title[i] == ' ' || title[i] == '\n' || title[i] == '\t') {    
                    std::cout << "ERROR: in map: \"" << title << "\", please do not put whitespace in title names\n";
                    exit(1);
                }
            }

            mapFile >> std::ws;

            while (mapFile.get(c))
            {
                if (c == '%')
                    break;
                else {
                    ++length;
                    world_in << c;
                }

                if (c == '\n')
                {
                    if (baseLength == -1)
                    {
                        baseLength = length;
                        length = 0;
                    }
                    else
                    {
                        if (length != baseLength)
                        {
                            std::cout << "ERROR: in map: \"" << title << "\" rows are not all the same length";
                            if (c == '\n' && prev == '\n')
                                std::cout << " (did you forget \% at the end?)\n";
                            else
                                std::cout << '\n';
                            exit(1);
                        }

                        length = 0;
                    }
                }

                prev = c;
            }

            world = world_in.str();
            worlds.insert(StrStrPair(title, world));
        }
    }
    mapFile.close();
}