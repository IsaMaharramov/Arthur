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

        player.Update(deltaTime, map);

        map.UpdateEnemies(player.x, player.y, player.health, deltaTime);

        BeginDrawing();

        ClearBackground({40, 40, 40, 255});
        DrawRectangle(0, 300, 800, 300, {20, 20, 20, 255});

        engine.DrawGame(player, map, deltaTime);

        EndDrawing();
    }

    engine.Unload();
    return 0;
}