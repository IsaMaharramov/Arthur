#include "raylib.h"
#include "Engine.h"
#include "Player.h"
#include "Map.h"

int main()
{
    Engine engine;
    Player player;
    Map map;

    engine.Init();

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        if (player.killCount >= 8 && map.currentLevel == 1)
        {
            map.LoadLevel(2);
            player.killCount = 0;
            player.x = 2.5f;
            player.y = 2.5f;
            player.health = 100;
        }
        else if (player.killCount >= 10 && map.currentLevel == 2)
        {
            map.LoadLevel(3);
            player.killCount = 0;
            player.x = 2.5f;
            player.y = 2.5f;
            player.health = 100;
        }
        else if (player.killCount >= 20 && map.currentLevel == 3)
        {
            map.LoadLevel(4);
            player.killCount = 0;
            player.x = 2.5f;
            player.y = 2.5f;
            player.health = 100;
        }
        else if (player.killCount >= 25 && map.currentLevel == 4)
        {
            map.LoadLevel(5);
            player.killCount = 0;
            player.x = 2.5f;
            player.y = 2.5f;
            player.health = 100;
        }

        if (!(map.currentLevel == 5 && player.killCount >= 40))
        {
            player.Update(deltaTime, map);
            map.UpdateEnemies(player.x, player.y, player.health, deltaTime);
        }

        BeginDrawing();

        ClearBackground({40, 40, 40, 255});
        DrawRectangle(0, 300, 800, 300, {20, 20, 20, 255});

        engine.DrawGame(player, map, deltaTime);

        EndDrawing();
    }

    engine.Unload();
    return 0;
}