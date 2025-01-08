#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define u8 uint8_t
#define u32 uint32_t
#define s32 int32_t
#define b32 s32
#define f32 float
#define f64 double

#include "string.h"
#include "string.c"

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

String read_file(char *path) {
    String result = {0};
    FILE *f = fopen(path, "rb");
    fseek(f, 0, SEEK_END);
    result.size = ftell(f);
    fseek(f, 0, SEEK_SET);
    result.data = malloc(result.size);
    fread(result.data, result.size, 1, f);
    fclose(f);
    return result;
}

typedef struct Particle {
    f32 x;
    f32 y;
    f32 z;
    f32 v;
} Particle;

typedef struct Sim_Step {
    Particle *particles;
    u32 count;
} Sim_Step;

Sim_Step parse_sim_step(String csv) {
    String *lines = str_split(str_trim(csv), Str("\n"));
    Sim_Step step = {
        .particles = malloc(arrlen(lines) * sizeof(Particle)),
        .count = arrlen(lines)
    };

    // Starting at i=1, because first line has column names
    for (u32 i = 1; i < arrlen(lines); ++i) {
        String line = lines[i];
        String *values = str_split(line, Str(","));
        assert(arrlen(values) == 4);
        step.particles[i] = (Particle){
            .x = str_to_f32(values[0]),
            .y = str_to_f32(values[1]),
            .z = str_to_f32(values[2]),
            .v = str_to_f32(values[3])
        };
        arrfree(values);
    }
    arrfree(lines);
    return step;
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

    String csv = read_file("res/mdsimulation/collision.0.csv");
    Sim_Step step = parse_sim_step(csv);
    free(csv.data);

    Image circle_image = GenImageGradientRadial(10.0f, 10.f, 1.0f, WHITE, BLANK);
    printf("format: %d\n", circle_image.format);
    Texture2D circle_tex = LoadTextureFromImage(circle_image);
    UnloadImage(circle_image);
    Shader alpha_shader = LoadShader(NULL, "res/alpha.fs");

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        update_camera_custom(&camera);

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);

        // BeginShaderMode(alpha_shader);
        // BeginBlendMode(BLEND_ADDITIVE);
        for (u32 i = 0; i < step.count; ++i) {
            Particle p = step.particles[i];
            DrawBillboard(camera, circle_tex, (Vector3){p.x, p.y, p.z}, 1.0f, BLUE);
        }
        // EndBlendMode();
        // EndShaderMode();

        // DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
        // DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
        // DrawGrid(10, 1.0f);

        EndMode3D();

        DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadTexture(circle_tex);

    CloseWindow();

    return 0;
}
