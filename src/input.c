void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        state.input.keyboard.keys[key].down = true;
    } else if (action == GLFW_RELEASE) {
        state.input.keyboard.keys[key].down = false;
        state.input.keyboard.keys[key].repeat = false;
    } else if (action == GLFW_REPEAT) {
        state.input.keyboard.keys[key].repeat = true;
    }
}

void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos) {
    state.input.mouse.pos = vec2(xpos, ypos);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        state.input.mouse.buttons[button].down = true;
        if (nk_item_is_any_active(state.window.nk_ctx)) state.input.mouse.buttons[button].ui_interaction = true;
    } else if (action == GLFW_RELEASE) {
        state.input.mouse.buttons[button].down = false;
        state.input.mouse.buttons[button].ui_interaction = false;
    }
    nk_glfw3_mouse_button_callback(window, button, action, mods);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    state.input.mouse.wheel = vec2(xoffset, yoffset);
    nk_gflw3_scroll_callback(window, xoffset, yoffset);
}

void char_callback(GLFWwindow *window, unsigned int codepoint) {
    nk_glfw3_char_callback(window, codepoint);
}

void reset_inputs() {
    for (s32 key = 0; key < GLFW_KEY_LAST; ++key) state.input.keyboard.keys[key].prev = state.input.keyboard.keys[key].down;
    for (s32 button = 0; button < GLFW_MOUSE_BUTTON_LAST; ++button) state.input.mouse.buttons[button].prev = state.input.mouse.buttons[button].down;
    state.input.mouse.prev_pos = state.input.mouse.pos;
    state.input.mouse.prev_wheel = state.input.mouse.wheel;
    state.input.mouse.wheel = vec2(0, 0);
}

vec2 get_mouse_pos() {
    return state.input.mouse.pos;
}

vec2 get_mouse_delta() {
    return v2_sub(state.input.mouse.pos, state.input.mouse.prev_pos);
}

b32 is_key_down(int key) {
    return state.input.keyboard.keys[key].down;
}

b32 is_key_held(int key) {
    return state.input.keyboard.keys[key].down && state.input.keyboard.keys[key].prev;
}

b32 is_key_repeated(int key) {
    return state.input.keyboard.keys[key].repeat;
}

b32 is_mouse_button_down(int button) {
    return state.input.mouse.buttons[button].down;
}

b32 is_mouse_button_held(int button) {
    return state.input.mouse.buttons[button].down && state.input.mouse.buttons[button].prev;
}

b32 is_mouse_button_ui_interaction(int button) {
    return state.input.mouse.buttons[button].ui_interaction;
}

void process_inputs(GLFWwindow *window) {
    if (is_key_down(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }
}
