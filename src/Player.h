#pragma once
#include <math.h>
#include "Map.h"
#include "raylib.h"

class Player
{
public:
    float x = 14.7f;
    float y = 5.09f;
    float angle = 0.0f;
    float fov = 3.14159f / 4.0f; // 45 degrees
    float speed = 5.0f;
    float turnSpeed = 3.0f;

    void Update(float deltaTime, Map &map)
    {
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
    }
};