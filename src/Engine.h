#pragma once
#include "raylib.h"
#include "Player.h"
#include "Map.h"
#include <math.h>
#include <vector>

class Engine
{
private:
    int screenWidth = 800;
    int screenHeight = 600;
    float maxDepth = 16.0f;

    Texture2D wallTexture;
    Texture2D enemySpriteSheet;
    int enemyFrameCount = 11;

public:
    void Init()
    {
        InitWindow(screenWidth, screenHeight, "Arthur Engine v1.0");
        SetTargetFPS(60);

        const char *wallPath = FileExists("../assets/wall.png") ? "../assets/wall.png" : "assets/wall.png";

        if (FileExists(wallPath))
        {
            wallTexture = LoadTexture(wallPath);
            SetTextureWrap(wallTexture, TEXTURE_WRAP_REPEAT);
        }
        else
        {
            Image img = GenImageChecked(64, 64, 8, 8, RED, MAROON);
            wallTexture = LoadTextureFromImage(img);
            UnloadImage(img);
        }

        const char *spritePath = FileExists("../assets/wizard idle.png") ? "../assets/wizard idle.png" : "assets/wizard idle.png";

        if (FileExists(spritePath))
        {
            Image img = LoadImage(spritePath);

            Color *pixels = LoadImageColors(img);
            Color bgColor = pixels[0];
            UnloadImageColors(pixels);

            Color transparent = {0, 0, 0, 0};
            ImageColorReplace(&img, bgColor, transparent);

            enemySpriteSheet = LoadTextureFromImage(img);
            UnloadImage(img);

            SetTextureFilter(enemySpriteSheet, TEXTURE_FILTER_POINT);
        }
        else
        {
            Image img = GenImageChecked(64, 64, 8, 8, DARKPURPLE, BLACK);
            enemySpriteSheet = LoadTextureFromImage(img);
            UnloadImage(img);
            enemyFrameCount = 1;
        }
    }

    void Unload()
    {
        UnloadTexture(wallTexture);
        UnloadTexture(enemySpriteSheet);
        CloseWindow();
    }

    void DrawGame(Player &player, Map &map, float deltaTime)
    {
        std::vector<float> depthBuffer(screenWidth, 0.0f);

        for (int x = 0; x < screenWidth; x++)
        {
            float rayAngle = (player.angle - player.fov / 2.0f) + ((float)x / (float)screenWidth) * player.fov;
            float distanceToWall = 0.0f;
            bool hitWall = false;
            int hitSide = 0;

            float eyeX = sinf(rayAngle);
            float eyeY = cosf(rayAngle);

            while (!hitWall && distanceToWall < maxDepth)
            {
                distanceToWall += 0.01f;
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
                    float blockMidX = (player.x + eyeX * distanceToWall) - (float)testX;
                    if (blockMidX < 0.02f || blockMidX > 0.98f)
                        hitSide = 1;
                }
            }

            depthBuffer[x] = distanceToWall;

            float correctedDistance = distanceToWall * cosf(rayAngle - player.angle);
            if (correctedDistance < 0.1f)
                correctedDistance = 0.1f;

            int ceiling = (int)((float)(screenHeight / 2.0f) - (float)screenHeight / correctedDistance);
            int floor = screenHeight - ceiling;
            int wallHeight = floor - ceiling;

            float intersectionX = player.x + eyeX * distanceToWall;
            float intersectionY = player.y + eyeY * distanceToWall;
            float textureXSample = (hitSide == 1) ? intersectionY : intersectionX;
            textureXSample -= floorf(textureXSample);

            int texX = (int)(textureXSample * (float)wallTexture.width);
            if ((hitSide == 0 && eyeX > 0) || (hitSide == 1 && eyeY < 0))
                texX = wallTexture.width - texX - 1;
            if (texX < 0)
                texX = 0;
            if (texX >= wallTexture.width)
                texX = wallTexture.width - 1;

            Rectangle sourceRect = {(float)texX, 0.0f, 1.0f, (float)wallTexture.height};
            Rectangle destRect = {(float)x, (float)ceiling, 1.0f, (float)wallHeight};

            float shadeMultiplier = (hitSide == 1) ? 0.65f : 1.0f;
            float depthShade = 1.0f - (distanceToWall / maxDepth);
            if (depthShade < 0.1f)
                depthShade = 0.1f;

            unsigned char finalShade = (unsigned char)(255.0f * depthShade * shadeMultiplier);
            Color tint = {finalShade, finalShade, finalShade, 255};
            DrawTexturePro(wallTexture, sourceRect, destRect, {0, 0}, 0.0f, tint);
        }

        for (auto &enemy : map.enemies)
        {
            if (!enemy.alive)
                continue;

            float dx = enemy.x - player.x;
            float dy = enemy.y - player.y;
            float dist = sqrtf(dx * dx + dy * dy);

            float enemyAngle = atan2f(dx, dy);
            float angleDiff = enemyAngle - player.angle;

            while (angleDiff < -3.14159f)
                angleDiff += 2.0f * 3.14159f;
            while (angleDiff > 3.14159f)
                angleDiff -= 2.0f * 3.14159f;

            if (fabs(angleDiff) < player.fov)
            {
                float screenX = (0.5f * (angleDiff / (player.fov / 2.0f)) + 0.5f) * screenWidth;

                float correctedDist = dist * cosf(angleDiff);
                if (correctedDist < 0.1f)
                    correctedDist = 0.1f;
                int enemyHeight = (int)((screenHeight / correctedDist) * 2.0f);

                int frameWidth = enemySpriteSheet.width / enemyFrameCount;
                float aspectRatio = (float)frameWidth / enemySpriteSheet.height;
                int enemyWidth = (int)((float)enemyHeight * aspectRatio);

                int startY = (screenHeight - enemyHeight) / 2;
                int startX = (int)screenX - (enemyWidth / 2);

                enemy.timeInFrame += deltaTime;
                if (enemy.timeInFrame > 0.12f)
                {
                    enemy.timeInFrame = 0.0f;
                    enemy.currentFrame = (enemy.currentFrame + 1) % enemyFrameCount;
                }

                int sourceX = enemy.currentFrame * frameWidth;
                unsigned char shade = (unsigned char)(255.0f - (dist / maxDepth) * 150.0f);
                if (dist > maxDepth)
                    shade = 10;
                Color tint = {shade, shade, shade, 255};

                for (int ex = 0; ex < enemyWidth; ex++)
                {
                    int drawX = startX + ex;
                    if (drawX > 0 && drawX < screenWidth && dist < depthBuffer[drawX])
                    {
                        float frameXPercent = (float)ex / enemyWidth;
                        float sourceColX = (float)sourceX + frameXPercent * frameWidth;

                        Rectangle colSourceRect = {sourceColX, 0.0f, 1.0f, (float)enemySpriteSheet.height};
                        Rectangle colDestRect = {(float)drawX, (float)startY, 1.0f, (float)enemyHeight};

                        DrawTexturePro(enemySpriteSheet, colSourceRect, colDestRect, {0, 0}, 0.0f, tint);
                    }
                }
            }
        }

        if (player.isShooting)
            DrawRectangle(0, 0, screenWidth, screenHeight, {255, 255, 0, 100});

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

        for (auto &enemy : map.enemies)
        {
            if (enemy.alive)
            {
                int enMapX = mapOffsetX + (int)(enemy.x * (float)mapScale);
                int enMapY = mapOffsetY + (int)(enemy.y * (float)mapScale);
                DrawRectangle(enMapX - 1, enMapY - 1, 3, 3, RED);
            }
        }
    }
};