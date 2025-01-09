#ifndef STATE_H
#define STATE_H

typedef struct State {
    struct {
        struct {
            b32 key_state[GLFW_KEY_LAST];
            b32 prev_key_state[GLFW_KEY_LAST];
        } keyboard;
        struct {
            vec2 pos;
            vec2 prev_pos;
            vec2 wheel;
            b32 button_state[GLFW_MOUSE_BUTTON_LAST];
            b32 prev_button_state[GLFW_MOUSE_BUTTON_LAST];
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
        ivec2 render;
    } window;
} State;

State state = {0};

#endif // STATE_H
