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

        BeginDrawing();

        ClearBackground({40, 40, 40, 255});
        DrawRectangle(0, 300, 800, 300, {20, 20, 20, 255});

        engine.DrawGame(player, map);

        DrawLine(400 - 8, 300, 400 + 8, 300, GREEN);
        DrawLine(400, 300 - 8, 400, 300 + 8, GREEN);

        DrawText("ARTHUR", 10, 10, 24, GOLD);
        DrawFPS(720, 10);

        EndDrawing();
    }

    engine.Unload();
    return 0;
}