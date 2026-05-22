#pragma once
#include "raylib.h"
#include "Player.h"
#include "Map.h"
#include <math.h>

class Engine
{
private:
    int screenWidth = 800;
    int screenHeight = 600;
    float maxDepth = 16.0f;

public:
    void Init()
    {
        InitWindow(screenWidth, screenHeight, "Advanced FPS Engine");
        SetTargetFPS(60);
    }

    void Draw3D(Player &player, Map &map)
    {
        for (int x = 0; x < screenWidth; x++)
        {
            float rayAngle = (player.angle - player.fov / 2.0f) + ((float)x / (float)screenWidth) * player.fov;

            float distanceToWall = 0.0f;
            bool hitWall = false;

            float eyeX = sinf(rayAngle);
            float eyeY = cosf(rayAngle);

            while (!hitWall && distanceToWall < maxDepth)
            {
                distanceToWall += 0.05f;

                int testX = (int)(player.x + eyeX * distanceToWall);
                int testY = (int)(player.y + eyeY * distanceToWall);

                if (testX < 0 || testX >= map.width || testY < 0 || testY >= map.height)
                {
                    hitWall = true;
                    distanceToWall = maxDepth;
                }
                else if (map.IsWall(testX, testY))
                {
                    hitWall = true;
                }
            }

            float correctedDistance = distanceToWall * cosf(rayAngle - player.angle);

            int ceiling = (float)(screenHeight / 2.0) - screenHeight / (float)correctedDistance;
            int floor = screenHeight - ceiling;
            int wallHeight = floor - ceiling;

            unsigned char colorIntensity = (unsigned char)(255.0f - (distanceToWall / maxDepth) * 255.0f);
            Color wallColor = {colorIntensity, 0, 0, 255};

            DrawRectangle(x, ceiling, 1, wallHeight, wallColor);
        }
    }
};