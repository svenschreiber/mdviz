#include "raylib.h"
#include "raymath.h"

int main(int argc, char **argv) { 
    InitWindow(400, 224, "HPSC Project");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
