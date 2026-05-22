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

        ClearBackground(DARKGRAY); // ceiling
        DrawRectangle(0, 300, 800, 300, BLACK); // floor

        engine.Draw3D(player, map);

        DrawLine(400 - 10, 300, 400 + 10, 300, GREEN); // crosshair
        DrawLine(400, 300 - 10, 400, 300 + 10, GREEN);

        DrawFPS(10, 10); // HUD

        EndDrawing();
    }

    CloseWindow();
    return 0;
}