#pragma once
#include <math.h>
#include "Map.h"
#include "raylib.h"

class Player
{
public:
    float x = 2.5f;
    float y = 2.5f;
    float angle = 0.0f;
    float fov = 3.14159f / 4.0f;
    float speed = 5.0f;
    float turnSpeed = 3.0f;

    bool isShooting = false;
    int health = 100;
    int killCount = 0;

    void Update(float deltaTime, Map &map)
    {
        if (health <= 0)
            return;

        if (IsKeyDown(KEY_A))
            angle -= turnSpeed * deltaTime;
        if (IsKeyDown(KEY_D))
            angle += turnSpeed * deltaTime;

        float moveX = sinf(angle) * speed * deltaTime;
        float moveY = cosf(angle) * speed * deltaTime;

        if (IsKeyDown(KEY_W))
        {
            if (!map.IsWall((int)(x + moveX), (int)y))
                x += moveX;
            if (!map.IsWall((int)x, (int)(y + moveY)))
                y += moveY;
        }

        if (IsKeyDown(KEY_S))
        {
            if (!map.IsWall((int)(x - moveX), (int)y))
                x -= moveX;
            if (!map.IsWall((int)x, (int)(y - moveY)))
                y -= moveY;
        }

        isShooting = false;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            isShooting = true;

            for (auto &enemy : map.enemies)
            {
                if (enemy.state == STATE_DYING || enemy.state == STATE_DEAD)
                    continue;

                float dx = enemy.x - x;
                float dy = enemy.y - y;
                float distance = sqrtf(dx * dx + dy * dy);
                float enemyAngle = atan2f(dx, dy);
                float angleDiff = enemyAngle - angle;

                while (angleDiff < -3.14159f)
                    angleDiff += 2.0f * 3.14159f;
                while (angleDiff > 3.14159f)
                    angleDiff -= 2.0f * 3.14159f;

                if (fabs(angleDiff) < 0.15f && distance < 16.0f)
                {
                    bool wallBlocking = false;
                    float testDist = 0.0f;
                    float rayX = sinf(enemyAngle);
                    float rayY = cosf(enemyAngle);

                    while (testDist < distance)
                    {
                        testDist += 0.5f;
                        if (map.IsWall((int)(x + rayX * testDist), (int)(y + rayY * testDist)))
                        {
                            wallBlocking = true;
                            break;
                        }
                    }

                    if (!wallBlocking)
                    {
                        enemy.health -= 25;

                        if (enemy.health <= 0)
                        {
                            enemy.state = STATE_DYING;
                            enemy.currentFrame = 0;
                            enemy.timeInFrame = 0.0f;
                            killCount++;
                        }
                        else
                        {
                            enemy.state = STATE_TAKE_HIT;
                            enemy.currentFrame = 0;
                            enemy.timeInFrame = 0.0f;
                        }
                    }
                }
            }
        }
    }
};