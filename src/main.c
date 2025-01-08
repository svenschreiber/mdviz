#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"

// Defaults from rcamera.h
#define CAMERA_MOVE_SPEED                 5.4f       // Units per second
#define CAMERA_ROTATION_SPEED             0.03f
#define CAMERA_PAN_SPEED                  0.2f
#define CAMERA_MOUSE_MOVE_SENSITIVITY     0.003f

void update_camera_custom(Camera3D *camera) {
    Vector2 mousePositionDelta = GetMouseDelta();

    bool moveInWorldPlane = true;
    bool rotateAroundTarget = true;
    bool lockView = true;
    bool rotateUp = false;

    float cameraMoveSpeed = CAMERA_MOVE_SPEED*GetFrameTime();
    float cameraRotationSpeed = CAMERA_ROTATION_SPEED*GetFrameTime();
    float cameraPanSpeed = CAMERA_PAN_SPEED*GetFrameTime();

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        CameraYaw(camera, -mousePositionDelta.x*CAMERA_MOUSE_MOVE_SENSITIVITY, rotateAroundTarget);
        CameraPitch(camera, -mousePositionDelta.y*CAMERA_MOUSE_MOVE_SENSITIVITY, lockView, rotateAroundTarget, rotateUp);
    }

    if (IsKeyDown(KEY_W)) CameraMoveForward(camera, cameraMoveSpeed, moveInWorldPlane);
    if (IsKeyDown(KEY_A)) CameraMoveRight(camera, -cameraMoveSpeed, moveInWorldPlane);
    if (IsKeyDown(KEY_S)) CameraMoveForward(camera, -cameraMoveSpeed, moveInWorldPlane);
    if (IsKeyDown(KEY_D)) CameraMoveRight(camera, cameraMoveSpeed, moveInWorldPlane);
    CameraMoveToTarget(camera, -GetMouseWheelMove());
}

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
        update_camera_custom(&camera);

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);

        DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
        DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
        DrawGrid(10, 1.0f);

        EndMode3D();

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
