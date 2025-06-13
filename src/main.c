// This is the entry point for the application. It contains the initialization steps
// and main loop. Since this project is pretty small and I'm the only person working
// on it, I opted for a "unity build" approach. That is, using only one compilation 
// unit (main.c in this case), where every other header *and* implementation file 
// gets included. This yields a much faster compilation time, without depending on 
// complex build systems, such as cmake. 

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear.h"
#include "nuklear_glfw_gl3.h"
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#include <mdlib.h>

#include "main.h"
#include "string.h"
#include "util.h"
#include "math.h"
#include "shader.h"
#include "data_loader.h"
#include "draw.h"
#include "input.h"
#include "camera.h"
#include "time.h"
#include "state.h"
#include "sim.h"
#include "ui.h"

#include "string.c"
#include "util.c"
#include "math.c"
#include "shader.c"
#include "data_loader.c"
#include "draw.c"
#include "input.c"
#include "camera.c"
#include "time.c"
#include "sim.c"
#include "ui.c"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    state.window.render = ivec2(width, height);
    mat4 projection_matrix = m4_perspective(to_radians(90.0f), (f32)width / height, 10000.f, 0.1f);
    shader_use(state.shader);
    shader_set_mat4(state.shader, "projection", projection_matrix);
}

void register_window_callbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCharCallback(window, char_callback);
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

    // Window creation
    GLFWwindow *window = glfwCreateWindow( 1280, 720, "HPSC Project", NULL, NULL );
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // enable v-sync
    register_window_callbacks(window);

    // Load OpenGL extensions
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        printf("[GLAD] Failed to initialize OpenGL context\n");
        return -1;
    }
    printf("[GLAD] Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    // Init UI library
    state.window.glfw = (struct nk_glfw){0};
    struct nk_glfw *glfw = &state.window.glfw;
    struct nk_context *ctx = nk_glfw3_init(glfw, window, 0);
    state.window.nk_ctx = ctx;
    struct nk_colorf bg = {0.10f, 0.18f, 0.24f, };
    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
    {
        struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(glfw, &atlas);
        nk_glfw3_font_stash_end(glfw);
    }

    // Enable modification of point size through shader
    glEnable(GL_PROGRAM_POINT_SIZE);

    // Load points shader
    state.shader = load_shader_program("res/shader/points.vs", "res/shader/points.fs");

    // VAO and VBO for particle geometry
    init_sim_geometry();

    // Create 3D camera
    Camera cam = make_camera(vec3(0.0f, 0.0f, 150.0f), 0.0f, -90.0f);
    glfwGetFramebufferSize(window, &state.window.render.width, &state.window.render.height);

    // Create projection transformation matrix
    mat4 projection_matrix = m4_perspective(to_radians(90.0f), (f32)state.window.render.width / state.window.render.height, 1000.f, 0.1f);
    shader_use(state.shader);
    shader_set_mat4(state.shader, "projection", projection_matrix);

    // Cap frames per second at 60
    set_max_fps(60);

    // Initialize time
    update_time(); 

    // Initialize simulation parameters
    sim_init();

    // Update/Render loop
    while (!glfwWindowShouldClose(window)) {
        // Update frame time
        update_time();
        if (state.sim.playing) {
            state.sim.timer += get_time_delta();
        }

        // Poll window events
        glfwPollEvents();

        // Update and render only when needed (because of fps cap)
        if (should_redraw()) {
            nk_glfw3_new_frame(glfw);

            // Process 3D viewer inputs only when UI is not being used
            if (!nk_item_is_any_active(ctx)) {
                process_inputs(window);
                move_cam(&cam);
            }

            // Check if sim params were changed through the UI
            sim_check_for_param_changes();

            // Advance simulation
            f32 step_time = 1.0f / state.sim.steps_per_second;
            if (state.sim.timer > step_time) {
                sim_step();
                state.sim.geometry.changed = true;
                state.sim.timer = 0.0f;
            }

            // Clear framebuffer
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Set shader uniforms
            shader_use(state.shader);
            mat4 view_matrix = camera_get_view_matrix(&cam);
            shader_set_mat4(state.shader, "view", view_matrix);
            shader_set_vec3(state.shader, "cam_pos", cam.pos);
            shader_set_vec2(state.shader, "vel_bounds", state.sim.geometry.vel_bounds);

            // Draw particle points
            glBindVertexArray(state.sim.geometry.vao);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glDrawArrays(GL_POINTS, 0, state.sim.geometry.n_particles);

            // Update/Render UI
            ui_update();
            nk_glfw3_render(glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);

            // Swap frame buffer and reset frame variables
            glfwSwapBuffers(window);
            reset_frame_time();
            reset_inputs();
        }

    }

    // Free some objects
    md_cont_destroy(state.sim.cont);
    md_refl_bound_destroy(state.sim.x_boundary);
    md_refl_bound_destroy(state.sim.y_boundary);
    md_refl_bound_destroy(state.sim.z_boundary);
    md_vel_stoer_verlet_destroy(state.sim.vel_stoer_verlet);
    nk_glfw3_shutdown(glfw);
    glfwTerminate();
    return 0;
}
