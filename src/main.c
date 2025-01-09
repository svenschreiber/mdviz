#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#include "main.h"
#include "string.h"
#include "math.h"
#include "input.h"
#include "state.h"

#include "string.c"
#include "math.c"
#include "input.c"

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

int main(int argc, char** argv) {
    if (!glfwInit()) {
        printf("[GLFW] Failed to initialize GLFW\n");
        return -1;
    }

#ifdef __APPLE__
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    GLFWwindow *window = glfwCreateWindow( 1280, 720, "HPSC Project", NULL, NULL );
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        printf("[GLAD] Failed to initialize OpenGL context\n");
        return -1;
    }
    printf("[GLAD] Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    register_window_callbacks(window);

    String csv = read_file("res/mdsimulation/collision.0.csv");
    Sim_Step step = parse_sim_step(csv);
    free(csv.data);

    while (!glfwWindowShouldClose(window)) {
        process_inputs(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
