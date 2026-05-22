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

    Texture2D wallTexture;

public:
    void Init()
    {
        InitWindow(screenWidth, screenHeight, "Arthur Engine v1.0");
        SetTargetFPS(60);

        if (FileExists("assets/wall.png"))
        {
            wallTexture = LoadTexture("assets/wall.png");
        }
        else
        {
            Image img = GenImageChecked(64, 64, 8, 8, RED, MAROON);
            wallTexture = LoadTextureFromImage(img);
            UnloadImage(img);
        }
    }

    void Unload()
    {
        UnloadTexture(wallTexture);
        CloseWindow();
    }

    void DrawGame(Player &player, Map &map)
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
                distanceToWall += 0.02f;

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

            int ceiling = (int)((float)(screenHeight / 2.0f) - (float)screenHeight / correctedDistance);
            int floor = screenHeight - ceiling;
            int wallHeight = floor - ceiling;

            float intersectionX = player.x + eyeX * distanceToWall;
            float intersectionY = player.y + eyeY * distanceToWall;

            float blockMidX = intersectionX - floorf(intersectionX);
            float blockMidY = intersectionY - floorf(intersectionY);

            float textureXSample = 0.0f;

            if (fabsf(blockMidX) < 0.02f || fabsf(blockMidX) > 0.98f)
            {
                textureXSample = blockMidY;
            }
            else
            {
                textureXSample = blockMidX;
            }

            int texX = (int)(textureXSample * (float)wallTexture.width) % wallTexture.width;
            if (texX < 0)
                texX += wallTexture.width;

            Rectangle sourceRect = {(float)texX, 0.0f, 1.0f, (float)wallTexture.height};
            Rectangle destRect = {(float)x, (float)ceiling, 1.0f, (float)wallHeight};

            unsigned char shade = (unsigned char)(255.0f - (distanceToWall / maxDepth) * 180.0f);
            Color tint = {shade, shade, shade, 255};

            DrawTexturePro(wallTexture, sourceRect, destRect, {0, 0}, 0.0f, tint);
        }

        int mapScale = 6;
        int mapOffsetX = 10;
        int mapOffsetY = 40;

        for (int mx = 0; mx < map.width; mx++)
        {
            for (int my = 0; my < map.height; my++)
            {
                Color tileColor = map.IsWall(mx, my) ? GRAY : BLACK;
                DrawRectangle(mapOffsetX + mx * mapScale, mapOffsetY + my * mapScale, mapScale - 1, mapScale - 1, tileColor);
            }
        }

        int playerMapX = mapOffsetX + (int)(player.x * (float)mapScale);
        int playerMapY = mapOffsetY + (int)(player.y * (float)mapScale);
        DrawCircle(playerMapX, playerMapY, 3, GREEN);

        float targetLineX = (float)playerMapX + sinf(player.angle) * 12.0f;
        float targetLineY = (float)playerMapY + cosf(player.angle) * 12.0f;
        DrawLine(playerMapX, playerMapY, (int)targetLineX, (int)targetLineY, GREEN);
    }
};  