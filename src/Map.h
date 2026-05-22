#pragma once
#include <string>

class Map
{
public:
    int width = 16;
    int height = 16;
    std::string layout;

    Map()
    {
        layout += "#########.......";
        layout += "#...............";
        layout += "#.......########";
        layout += "#..............#";
        layout += "#......##......#";
        layout += "#......##......#";
        layout += "#..............#";
        layout += "###............#";
        layout += "##.............#";
        layout += "#......####..###";
        layout += "#......#.......#";
        layout += "#......#.......#";
        layout += "#..............#";
        layout += "#......#########";
        layout += "#..............#";
        layout += "################";
    }

    bool IsWall(int x, int y)
    {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return true;
        return layout[y * width + x] == '#';
    }
};