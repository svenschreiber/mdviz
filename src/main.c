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

// TODO: cleanup
typedef struct {
    GLuint vao;
    GLuint vbo;
} Sim_Geometry;

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

#include "string.c"
#include "util.c"
#include "math.c"
#include "shader.c"
#include "data_loader.c"
#include "draw.c"
#include "input.c"
#include "camera.c"
#include "time.c"

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

Sim_Step *get_selected_step() {
    return &state.seq.steps[state.seq.selected];
}

vec2 get_vel_bounds() {
    vec2 bounds = vec2(FLT_MAX, -FLT_MAX);
    for (u32 i = 0; i < state.sim.geometry.n_particles; ++i) {
        f32 v = state.sim.geometry.data[i].v;
        if (v < bounds.x) bounds.x = v;
        if (v > bounds.y) bounds.y = v;
    }
    return bounds;
}

void step_simulation() {
    md_vel_stoer_verlet_update_positions(state.sim.vel_stoer_verlet, state.sim.cont);

    // apply boundaries
    md_refl_bound_apply_cont(state.sim.x_boundary, state.sim.cont);
    md_refl_bound_apply_cont(state.sim.y_boundary, state.sim.cont);
    md_refl_bound_apply_cont(state.sim.z_boundary, state.sim.cont);

    // flush forces
    md_cont_flush_forces(state.sim.cont);

    // compute new forces
    md_ljforce_apply(state.sim.cont);

    // update velocities
    md_vel_stoer_verlet_update_velocities(state.sim.vel_stoer_verlet, state.sim.cont);
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

    String csv = read_entire_file("res/mdsimulation/collision.csv");
    state.seq = load_sim_sequence(csv);
    free(csv.data);

    glEnable(GL_PROGRAM_POINT_SIZE);
    state.shader = load_shader_program("res/shader/points.vs", "res/shader/points.fs");

    init_sim_geometry();

    Camera cam = make_camera(vec3(0.0f, 0.0f, 150.0f), 0.0f, -90.0f);
    glfwGetFramebufferSize(window, &state.window.render.width, &state.window.render.height);
    mat4 projection_matrix = m4_perspective(to_radians(90.0f), (f32)state.window.render.width / state.window.render.height, 1000.f, 0.1f);
    shader_use(state.shader);
    shader_set_mat4(state.shader, "projection", projection_matrix);

    set_max_fps(60);
    update_time(); // initialize time

    // Set initial simulation state
    // Problem:
    state.sim.problem.n        = ivec3(10, 10, 10);
    state.sim.problem.vel      = 0.5f;
    state.sim.problem.bounds_x = vec2(55.0f, -55.0f);
    state.sim.problem.bounds_y = vec2(55.0f, -55.0f);
    state.sim.problem.bounds_z = vec2(55.0f, -55.0f);
    state.sim.problem.changed  = true;

    // Reflective Boundaries
    state.sim.boundaries.x = vec2(-55.0f + 1e-3, 55.0f - 1e-3);
    state.sim.boundaries.y = vec2(-55.0f + 1e-3, 55.0f - 1e-3);
    state.sim.boundaries.z = vec2(-55.0f + 1e-3, 55.0f - 1e-3);
    state.sim.boundaries.changed = true;

    // Integrator
    state.sim.integrator.dt      = 1.0f;
    state.sim.integrator.changed = true;

    // Sim playback
    state.sim.steps_per_second = 30;

    while (!glfwWindowShouldClose(window)) {
        update_time();
        if (state.sim.playing) {
            state.sim.timer += get_time_delta();
        }

        glfwPollEvents();

        if (should_redraw()) {
            nk_glfw3_new_frame(glfw);
            if (!nk_item_is_any_active(ctx)) {
                process_inputs(window);
                move_cam(&cam);
            }

            if (state.sim.problem.changed) {
                if (state.sim.cont) md_cont_destroy(state.sim.cont);
                state.sim.cont = md_get_aos_problem(
                    state.sim.problem.n.x, state.sim.problem.n.y, state.sim.problem.n.z, 
                    state.sim.problem.vel,
                    state.sim.problem.bounds_x.x, state.sim.problem.bounds_x.y, 
                    state.sim.problem.bounds_y.x, state.sim.problem.bounds_y.y, 
                    state.sim.problem.bounds_z.x, state.sim.problem.bounds_z.y
                );
                // init forces
                md_ljforce_apply(state.sim.cont);

                state.sim.geometry.changed = true;
                state.sim.problem.changed = false;
            }

            if (state.sim.boundaries.changed) {
                md_refl_bound_destroy(state.sim.x_boundary);
                md_refl_bound_destroy(state.sim.y_boundary);
                md_refl_bound_destroy(state.sim.z_boundary);
                state.sim.x_boundary = md_refl_bound_create(state.sim.boundaries.x.x, state.sim.boundaries.x.y, 0);
                state.sim.y_boundary = md_refl_bound_create(state.sim.boundaries.y.x, state.sim.boundaries.y.y, 1);
                state.sim.z_boundary = md_refl_bound_create(state.sim.boundaries.z.x, state.sim.boundaries.z.y, 2);
                state.sim.boundaries.changed = false;
            }

            if (state.sim.integrator.changed) {
                md_vel_stoer_verlet_destroy(state.sim.vel_stoer_verlet);
                state.sim.vel_stoer_verlet = md_vel_stoer_verlet_create(state.sim.integrator.dt);
                state.sim.integrator.changed = false;
            }

            if (state.sim.geometry.changed) {
                u32 n_particles = md_cont_size(state.sim.cont);
                if (state.sim.geometry.n_particles != n_particles) {
                    if (state.sim.geometry.data) free(state.sim.geometry.data);
                    state.sim.geometry.data = ALLOC(Particle, n_particles);
                    state.sim.geometry.n_particles = n_particles;
                    alloc_vbo_data(n_particles);
                }

                md_cont_get_data(state.sim.cont, (f32 *)state.sim.geometry.data);
                update_vbo_data();

                state.sim.geometry.vel_bounds = get_vel_bounds();

                state.sim.geometry.changed = false;
            }

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shader_use(state.shader);
            mat4 view_matrix = camera_get_view_matrix(&cam);
            shader_set_mat4(state.shader, "view", view_matrix);
            shader_set_vec3(state.shader, "cam_pos", cam.pos);
            shader_set_vec2(state.shader, "vel_bounds", state.sim.geometry.vel_bounds);

            glBindVertexArray(state.sim.geometry.vao);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glDrawArrays(GL_POINTS, 0, state.sim.geometry.n_particles);


            if (nk_begin(ctx, "Controls", nk_rect(50, 50, 230, 250),
                         NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
                         NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
            {
                nk_layout_row_static(ctx, 30, 80, 1);
                if (!state.sim.playing) {
                    if (nk_button_label(ctx, "Play")) {
                        state.sim.playing = true;
                        state.sim.timer = 0.0f;
                    }
                } else {
                    if (nk_button_label(ctx, "Pause")) {
                        state.sim.playing = false;
                    }
                }

                f32 step_time = 1.0f / state.sim.steps_per_second;
                if (state.sim.timer > step_time) {
                    step_simulation();
                    state.sim.geometry.changed = true;
                    state.sim.timer = 0.0f;
                }

                nk_layout_row_dynamic(ctx, 25, 1);
                nk_value_float(ctx, "Frame time:", state.time.frame_dt * 1000);
                nk_label(ctx, "Problem", NK_TEXT_LEFT);
                if (nk_property_int(ctx, "x:", 0, &state.sim.problem.n.x, 1000, 1, 1)) state.sim.problem.changed = true;
                if (nk_property_int(ctx, "y:", 0, &state.sim.problem.n.y, 1000, 1, 1)) state.sim.problem.changed = true;
                if (nk_property_int(ctx, "z:", 0, &state.sim.problem.n.z, 1000, 1, 1)) state.sim.problem.changed = true;
            }
            nk_end(ctx);

            nk_glfw3_render(glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);

            glfwSwapBuffers(window);
            reset_frame_time();
            reset_inputs();
        }

    }

    md_cont_destroy(state.sim.cont);
    md_refl_bound_destroy(state.sim.x_boundary);
    md_refl_bound_destroy(state.sim.y_boundary);
    md_refl_bound_destroy(state.sim.z_boundary);
    md_vel_stoer_verlet_destroy(state.sim.vel_stoer_verlet);
    nk_glfw3_shutdown(glfw);
    glfwTerminate();
    return 0;
}
