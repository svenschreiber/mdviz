#include "raylib.h"
#include "raymath.h"

int main(int argc, char **argv) { 
    InitWindow(1280, 720, "HPSC Project");

    Camera3D camera = {
        .position   = (Vector3){0.0f, 10.0f, 10.0f},
        .target     = (Vector3){0.0f, 0.0f, 0.0f},
        .up         = (Vector3){0.0f, 1.0f, 0.0f},
        .fovy       = 45.0f,
        .projection = CAMERA_PERSPECTIVE
    };

    Vector3 cubePosition = {0.0f, 0.0f, 0.0f};

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);

        DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
        DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
        DrawGrid(10, 1.0f);

        EndMode3D();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
