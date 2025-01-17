#ifndef STATE_H
#define STATE_H

typedef struct State {
    struct {
        struct {
            struct {
                b32 down;
                b32 prev;
                b32 repeat;
            } keys[GLFW_KEY_LAST];
        } keyboard;
        struct {
            vec2 pos;
            vec2 prev_pos;
            vec2 wheel;
            vec2 prev_wheel;
            struct {
                b32 down;
                b32 prev;
                b32 ui_interaction;
            } buttons[GLFW_MOUSE_BUTTON_LAST];
        } mouse;
    } input;
    struct {
        f32 current;
        f32 prev;
        f32 delta;
        f32 max_fps;
        f32 min_frame_time;
        f32 frame_dt;
    } time;
    struct {
        struct nk_context *nk_ctx;
        struct nk_glfw glfw;
        ivec2 render;
    } window;
    struct {
        s32 sim_step;
    } ui;
    struct {
        MD_ContainerType type;
        MD_ParticleContainer *cont;
        MD_ReflectiveBoundary *x_boundary;
        MD_ReflectiveBoundary *y_boundary;
        MD_ReflectiveBoundary *z_boundary;
        MD_VelocityStoermerVerlet *vel_stoer_verlet;
        struct {
            ivec3 n;
            f32 vel;
            vec2 bounds_x;
            vec2 bounds_y;
            vec2 bounds_z;
            b32 changed;
        } problem;
        struct {
            vec2 x;
            vec2 y;
            vec2 z;
            b32 changed;
        } boundaries;
        struct {
            f32 dt;
            b32 changed;
        } integrator;
        struct {
            GLuint vao;
            GLuint vbo;
            Particle *data;
            u32 n_particles;
            vec2 vel_bounds;
            b32 changed;
        } geometry;

        // Playback
        b32 playing;
        f32 timer;
        s32 steps_per_second;
    } sim;

    GLuint shader;
    Sim_Sequence seq;
    Sim_Geometry geom;
} State;

State state = {0};

#endif // STATE_H
