#pragma once
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "Enemy.h"

class Map
{
public:
    int width;
    int height;
    std::string layout;
    std::vector<Enemy> enemies;
    int currentLevel = 1;

    Map()
    {
        srand((unsigned int)time(NULL));
        LoadLevel(1);
    }

    void LoadLevel(int level)
    {
        currentLevel = level;
        layout = "";
        enemies.clear();

        if (level == 1)
        {
            width = 32;
            height = 32;
            layout += "################################";
            layout += "#..............#...............#";
            layout += "#......##......#.......#####...#";
            layout += "#......##..............#.......#";
            layout += "#..............#...............#";
            layout += "###............#.......#...#####";
            layout += "##.............#.......#.......#";
            layout += "#......####..###.......#.......#";
            layout += "#......#.......#.......#####...#";
            layout += "#......#.......................#";
            layout += "#..............#.......#########";
            layout += "#......#########...............#";
            layout += "#..............#...............#";
            layout += "############.###..#..###########";
            layout += "#.................#............#";
            layout += "#..#########.#######.......#...#";
            layout += "#.................#........#...#";
            layout += "#.................#........#...#";
            layout += "#######..###.######........#...#";
            layout += "#.................#........#...#";
            layout += "#...########.######..#######...#";
            layout += "#...#..........................#";
            layout += "#...#......########..###########";
            layout += "#...#......#...................#";
            layout += "#...#......#.........###########";
            layout += "#...#..........................#";
            layout += "#...########.........##..##..###";
            layout += "#..........#.........##..##..###";
            layout += "########.......................#";
            layout += "#..........#...................#";
            layout += "#..........#...................#";
            layout += "################################";

            for (int i = 0; i < 10; i++)
                SpawnRandomEnemy(TYPE_WIZARD);
        }
        else if (level == 2)
        {
            width = 40;
            height = 40;
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
                        layout += "#";
                    else if (i % 4 == 0 && j % 5 != 0)
                        layout += "#";
                    else
                        layout += ".";
                }
            }
            for (int i = 0; i < 6; i++)
                SpawnRandomEnemy(TYPE_GOBLIN);
            for (int i = 0; i < 4; i++)
                SpawnRandomEnemy(TYPE_WIZARD);
        }
        else if (level == 3)
        {
            width = 50;
            height = 50;
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
                        layout += "#";
                    else if (i % 6 == 0 && j % 6 == 0)
                        layout += "#";
                    else
                        layout += ".";
                }
            }
            for (int i = 0; i < 10; i++)
                SpawnRandomEnemy(TYPE_SKELETON);
            for (int i = 0; i < 5; i++)
                SpawnRandomEnemy(TYPE_GOBLIN);
            for (int i = 0; i < 5; i++)
                SpawnRandomEnemy(TYPE_WIZARD);
        }
        else if (level == 4)
        {
            width = 50;
            height = 50;
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
                        layout += "#";
                    else if ((i * j) % 9 == 0)
                        layout += "#";
                    else
                        layout += ".";
                }
            }
            for (int i = 0; i < 12; i++)
                SpawnRandomEnemy(TYPE_MUSHROOM);
            for (int i = 0; i < 8; i++)
                SpawnRandomEnemy(TYPE_SKELETON);
            for (int i = 0; i < 5; i++)
                SpawnRandomEnemy(TYPE_GOBLIN);
        }
        else if (level == 5)
        {
            width = 60;
            height = 60;
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
                        layout += "#";
                    else if (i % 12 == 0 && j % 12 == 0)
                        layout += "#";
                    else
                        layout += ".";
                }
            }
            for (int i = 0; i < 15; i++)
                SpawnRandomEnemy(TYPE_EYE);
            for (int i = 0; i < 10; i++)
                SpawnRandomEnemy(TYPE_MUSHROOM);
            for (int i = 0; i < 5; i++)
                SpawnRandomEnemy(TYPE_SKELETON);
            for (int i = 0; i < 5; i++)
                SpawnRandomEnemy(TYPE_GOBLIN);
            for (int i = 0; i < 5; i++)
                SpawnRandomEnemy(TYPE_WIZARD);
        }
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
            if (enemy.state == STATE_DEAD || enemy.state == STATE_DYING || enemy.state == STATE_TAKE_HIT)
                continue;

            float dx = playerX - enemy.x;
            float dy = playerY - enemy.y;
            float distance = sqrtf(dx * dx + dy * dy);

            float moveSpeed = 1.5f;
            int attackDamage = 15;
            float attackCooldown = 1.0f;

            if (enemy.type == TYPE_GOBLIN)
            {
                moveSpeed = 2.5f;
                attackDamage = 10;
                attackCooldown = 0.6f;
            }
            else if (enemy.type == TYPE_SKELETON)
            {
                moveSpeed = 1.0f;
                attackDamage = 25;
                attackCooldown = 1.5f;
            }
            else if (enemy.type == TYPE_MUSHROOM)
            {
                moveSpeed = 1.8f;
                attackDamage = 20;
                attackCooldown = 1.0f;
            }
            else if (enemy.type == TYPE_EYE)
            {
                moveSpeed = 3.5f;
                attackDamage = 5;
                attackCooldown = 0.4f;
            }

            if (distance < 1.5f)
            {
                enemy.state = STATE_ATTACK;
                enemy.aiTimer -= deltaTime;
                enemy.isWandering = false;

                if (enemy.aiTimer <= 0.0f)
                {
                    playerHealth -= attackDamage;
                    if (playerHealth < 0)
                        playerHealth = 0;
                    enemy.aiTimer = attackCooldown;
                }
            }
            else if (distance < 15.0f && distance > 0.5f)
            {
                enemy.state = STATE_FLYING;
                enemy.aiTimer = 0.5f;
                enemy.isWandering = false;

                float moveX = (dx / distance) * moveSpeed * deltaTime;
                float moveY = (dy / distance) * moveSpeed * deltaTime;

                if (!IsWall((int)(enemy.x + moveX), (int)enemy.y))
                    enemy.x += moveX;
                if (!IsWall((int)enemy.x, (int)(enemy.y + moveY)))
                    enemy.y += moveY;
            }
            else
            {
                enemy.aiTimer -= deltaTime;
                if (enemy.aiTimer <= 0.0f)
                {
                    enemy.isWandering = !enemy.isWandering;
                    if (enemy.isWandering)
                    {
                        enemy.wanderAngle = (float)(rand() % 360) * (3.14159f / 180.0f);
                        enemy.aiTimer = 1.0f + (rand() % 20) / 10.0f;
                    }
                    else
                    {
                        enemy.aiTimer = 2.0f + (rand() % 80) / 10.0f;
                    }
                }

                if (enemy.isWandering)
                {
                    enemy.state = STATE_FLYING;
                    float moveX = sinf(enemy.wanderAngle) * (moveSpeed * 0.5f) * deltaTime;
                    float moveY = cosf(enemy.wanderAngle) * (moveSpeed * 0.5f) * deltaTime;

                    bool hitWallX = IsWall((int)(enemy.x + moveX), (int)enemy.y);
                    bool hitWallY = IsWall((int)enemy.x, (int)(enemy.y + moveY));

                    if (!hitWallX)
                        enemy.x += moveX;
                    if (!hitWallY)
                        enemy.y += moveY;

                    if (hitWallX || hitWallY)
                    {
                        enemy.isWandering = false;
                        enemy.aiTimer = 1.0f + (rand() % 50) / 10.0f;
                    }
                }
                else
                {
                    enemy.state = STATE_IDLE;
                }
            }
        }
    }

private:
    void SpawnRandomEnemy(EnemyType type)
    {
        int rx, ry;
        do
        {
            rx = rand() % width;
            ry = rand() % height;
        } while (IsWall(rx, ry) || (rx < 5 && ry < 5));
        SpawnEnemy((float)rx + 0.5f, (float)ry + 0.5f, type);
    }

    void SpawnEnemy(float x, float y, EnemyType type)
    {
        if (!IsWall((int)x, (int)y))
        {
            float randomStartDelay = (rand() % 100) / 10.0f;

            int startingHealth = 50;
            if (type == TYPE_WIZARD)
                startingHealth = 75;
            else if (type == TYPE_GOBLIN)
                startingHealth = 50;
            else if (type == TYPE_SKELETON)
                startingHealth = 150;
            else if (type == TYPE_MUSHROOM)
                startingHealth = 100;
            else if (type == TYPE_EYE)
                startingHealth = 25;

            enemies.push_back({x, y, type, STATE_IDLE, 0, 0.0f, randomStartDelay, false, 0.0f, startingHealth});
        }
    }
};