#pragma once
#include <string>
#include <vector>
#include "Enemy.h"

class Map
{
public:
    int width = 16;
    int height = 16;
    std::string layout;

    std::vector<Enemy> enemies;

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

        SpawnEnemy(7.5f, 4.5f);
        SpawnEnemy(10.5f, 5.5f);
        SpawnEnemy(14.5f, 10.5f);
    }

    bool IsWall(int x, int y)
    {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return true;
        return layout[y * width + x] == '#';
    }

private:
    void SpawnEnemy(float x, float y)
    {
        if (!IsWall((int)x, (int)y))
        {
            enemies.push_back({x, y, true, 0, 0.0f});
        }
        else
        {
            // 
        }
    }
};