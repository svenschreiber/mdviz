void update_time() {
    state.time.prev = state.time.current;
    state.time.current = (f32)glfwGetTime();
    state.time.delta = state.time.current - state.time.prev;
    state.time.frame_dt += state.time.delta;
}

f32 get_time() {
    return state.time.current;
}

f32 get_time_delta() {
    return state.time.delta;
}

f32 get_frame_time() {
    return state.time.frame_dt;
}

void set_max_fps(f32 fps) {
    state.time.max_fps = fps;
    state.time.min_frame_time = 1 / fps;
}

b32 should_redraw() {
    return state.time.frame_dt >= state.time.min_frame_time;
}

void reset_frame_time() {
    state.time.frame_dt = 0;
}
