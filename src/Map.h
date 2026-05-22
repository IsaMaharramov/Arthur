#pragma once
#include <string>
#include <vector>
#include <math.h>
#include "Enemy.h"

class Player;

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

    void UpdateEnemies(float playerX, float playerY, int &playerHealth, float deltaTime)
    {
        if (playerHealth <= 0)
            return;

        for (auto &enemy : enemies)
        {
            if (enemy.state == STATE_DEAD || enemy.state == STATE_DYING)
                continue;

            float dx = playerX - enemy.x;
            float dy = playerY - enemy.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance < 1.5f)
            {
                enemy.state = STATE_ATTACK;
                enemy.aiTimer -= deltaTime;

                if (enemy.aiTimer <= 0.0f)
                {
                    playerHealth -= 15;
                    if (playerHealth < 0)
                        playerHealth = 0;
                    enemy.aiTimer = 1.0f;
                }
            }
            else if (distance < 8.0f)
            {
                enemy.state = STATE_FLYING;
                enemy.aiTimer = 0.5f;

                float moveX = (dx / distance) * 1.5f * deltaTime;
                float moveY = (dy / distance) * 1.5f * deltaTime;

                if (!IsWall((int)(enemy.x + moveX), (int)enemy.y))
                    enemy.x += moveX;
                if (!IsWall((int)enemy.x, (int)(enemy.y + moveY)))
                    enemy.y += moveY;
            }
            else
            {
                enemy.state = STATE_IDLE;
            }
        }
    }

private:
    void SpawnEnemy(float x, float y)
    {
        if (!IsWall((int)x, (int)y))
        {
            enemies.push_back({x, y, STATE_IDLE, 0, 0.0f, 0.0f});
        }
    }
};