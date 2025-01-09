#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#include "main.h"
#include "string.h"
#include "util.h"
#include "math.h"
#include "input.h"
#include "camera.h"
#include "time.h"
#include "state.h"
#include "shader.h"

#include "string.c"
#include "util.c"
#include "math.c"
#include "input.c"
#include "camera.c"
#include "time.c"
#include "shader.c"

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
        .particles = ALLOC(Particle, arrlen(lines)),
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    state.window.render = ivec2(width, height);
}

void register_window_callbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
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
    glfwSwapInterval(1);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        printf("[GLAD] Failed to initialize OpenGL context\n");
        return -1;
    }
    printf("[GLAD] Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    register_window_callbacks(window);

    String csv = read_entire_file("res/mdsimulation/collision.10.csv");
    Sim_Step step = parse_sim_step(csv);
    free(csv.data);

    GLuint shader = load_shader_program("res/shader/points.vs", "res/shader/points.fs");

    GLuint point_vao;
    glGenVertexArrays(1, &point_vao);
    glBindVertexArray(point_vao);
    GLuint point_vbo;
    glGenBuffers(1, &point_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, point_vbo);
    glBufferData(GL_ARRAY_BUFFER, step.count * sizeof(Particle), step.particles, GL_STATIC_DRAW);
    // f32 point_data[] = {
    //     0.0f, 0.0f, 0.0f, 1.0f,
    // };
    // glBufferData(GL_ARRAY_BUFFER, sizeof(point_data), point_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (GLvoid *)0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (GLvoid *)(3 * sizeof(f32)));

    glPointSize(2.0f);

    Camera cam = make_camera(vec3(0.0f, 0.0f, 150.0f), 0.0f, -90.0f);
    glfwGetFramebufferSize(window, &state.window.render.width, &state.window.render.height);
    mat4 projection_matrix = m4_perspective(to_radians(90.0f), (f32)state.window.render.width / state.window.render.height, 1000.f, 0.1f);
    shader_use(shader);
    shader_set_mat4(shader, "projection", projection_matrix);

    set_max_fps(60);
    update_time(); // to initialize time

    while (!glfwWindowShouldClose(window)) {
        update_time();

        glfwPollEvents();
        process_inputs(window);

        if (should_redraw()) {
            move_cam(&cam);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shader_use(shader);
            mat4 view_matrix = camera_get_view_matrix(&cam);
            shader_set_mat4(shader, "view", view_matrix);

            glBindVertexArray(point_vao);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glDrawArrays(GL_POINTS, 0, step.count);

            glfwSwapBuffers(window);
            reset_frame_time();
            state.input.mouse.wheel = vec2(0);
        }

    }

    glfwTerminate();
    return 0;
}
