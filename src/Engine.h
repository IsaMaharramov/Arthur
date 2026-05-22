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
    Texture2D texIdle;
    Texture2D texDeath;
    Texture2D texFly;
    Texture2D texAttack;

    Texture2D LoadWizardTexture(const char *localPath, const char *buildPath)
    {
        const char *actualPath = FileExists(buildPath) ? buildPath : (FileExists(localPath) ? localPath : nullptr);
        if (actualPath)
        {
            Image img = LoadImage(actualPath);
            Color *pixels = LoadImageColors(img);
            Color bgColor = pixels[0];
            UnloadImageColors(pixels);

            Color transparent = {0, 0, 0, 0};
            ImageColorReplace(&img, bgColor, transparent);

            Texture2D tex = LoadTextureFromImage(img);
            UnloadImage(img);
            SetTextureFilter(tex, TEXTURE_FILTER_POINT);
            return tex;
        }
        Image img = GenImageChecked(64, 64, 8, 8, MAGENTA, BLACK);
        Texture2D tex = LoadTextureFromImage(img);
        UnloadImage(img);
        return tex;
    }

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

        texIdle = LoadWizardTexture("assets/wizard idle.png", "../assets/wizard idle.png");
        texDeath = LoadWizardTexture("assets/wizard death.png", "../assets/wizard death.png");
        texFly = LoadWizardTexture("assets/wizard fly forward.png", "../assets/wizard fly forward.png");
        texAttack = LoadWizardTexture("assets/wizard attack.png", "../assets/wizard attack.png");
    }

    void Unload()
    {
        UnloadTexture(wallTexture);
        UnloadTexture(texIdle);
        UnloadTexture(texDeath);
        UnloadTexture(texFly);
        UnloadTexture(texAttack);
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
            if (enemy.state == STATE_DEAD)
                continue;

            Texture2D activeSprite = texIdle;
            int maxFrames = 10;
            float animSpeed = 0.12f;

            if (enemy.state == STATE_IDLE)
            {
                activeSprite = texIdle;
                maxFrames = 10;
            }
            else if (enemy.state == STATE_FLYING)
            {
                activeSprite = texFly;
                maxFrames = 6;
                animSpeed = 0.10f;
            }
            else if (enemy.state == STATE_DYING)
            {
                activeSprite = texDeath;
                maxFrames = 10;
                animSpeed = 0.20f;
            }
            else if (enemy.state == STATE_ATTACK)
            {
                activeSprite = texAttack;
                maxFrames = 8;
                animSpeed = 0.12f;
            }

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

                int frameWidth = activeSprite.width / maxFrames;
                float aspectRatio = (float)frameWidth / activeSprite.height;
                int enemyWidth = (int)((float)enemyHeight * aspectRatio);

                int startY = (screenHeight - enemyHeight) / 2;
                int startX = (int)screenX - (enemyWidth / 2);

                enemy.timeInFrame += deltaTime;
                if (enemy.timeInFrame > animSpeed)
                {
                    enemy.timeInFrame = 0.0f;
                    enemy.currentFrame++;

                    if (enemy.currentFrame >= maxFrames)
                    {
                        if (enemy.state == STATE_DYING)
                            enemy.state = STATE_DEAD;
                        else
                            enemy.currentFrame = 0;
                    }
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

                        Rectangle colSourceRect = {sourceColX, 0.0f, 1.0f, (float)activeSprite.height};
                        Rectangle colDestRect = {(float)drawX, (float)startY, 1.0f, (float)enemyHeight};

                        DrawTexturePro(activeSprite, colSourceRect, colDestRect, {0, 0}, 0.0f, tint);
                    }
                }
            }
        }

        if (player.isShooting)
            DrawRectangle(0, 0, screenWidth, screenHeight, {255, 255, 0, 100});

        DrawLine(400 - 8, 300, 400 + 8, 300, GREEN);
        DrawLine(400, 300 - 8, 400, 300 + 8, GREEN);

        DrawText("ARTHUR", 10, 10, 24, GOLD);
        DrawFPS(720, 10);

        Color healthColor = (player.health > 40) ? GREEN : RED;
        DrawText(TextFormat("HEALTH: %d", player.health), screenWidth / 2 - 80, screenHeight - 50, 30, healthColor);

        if (player.health <= 0)
        {
            DrawRectangle(0, 0, screenWidth, screenHeight, {150, 0, 0, 180}); // Dark transparent red
            DrawText("YOU DIED", screenWidth / 2 - 120, screenHeight / 2 - 30, 50, BLACK);
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
        DrawCircle(mapOffsetX + (int)(player.x * mapScale), mapOffsetY + (int)(player.y * mapScale), 3, GREEN);

        for (auto &enemy : map.enemies)
        {
            if (enemy.state != STATE_DEAD)
            {
                DrawRectangle(mapOffsetX + (int)(enemy.x * mapScale) - 1, mapOffsetY + (int)(enemy.y * mapScale) - 1, 3, 3, RED);
            }
        }
    }
};