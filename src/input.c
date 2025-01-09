void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    state.input.keyboard.prev_key_state[key] = state.input.keyboard.key_state[key];
    if (action == GLFW_PRESS) {
        state.input.keyboard.key_state[key] = true;
    } else if (action == GLFW_RELEASE) {
        state.input.keyboard.key_state[key] = false;
    }
}

void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos) {
    state.input.mouse.prev_pos = state.input.mouse.pos;
    state.input.mouse.pos = vec2(xpos, ypos);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    state.input.mouse.prev_button_state[button] = state.input.mouse.button_state[button];
    if (action == GLFW_PRESS) {
        state.input.mouse.button_state[button] = true;
    } else if (action == GLFW_RELEASE) {
        state.input.mouse.button_state[button] = false;
    }
}

void register_window_callbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    state.input.mouse.wheel = vec2(xoffset, yoffset);
}

vec2 get_mouse_pos() {
    return state.input.mouse.pos;
}

vec2 get_mouse_delta() {
    return v2_sub(state.input.mouse.pos, state.input.mouse.prev_pos);
}

b32 is_key_down(int key) {
    return state.input.keyboard.key_state[key];
}

b32 is_key_held(int key) {
    return state.input.keyboard.key_state[key] && state.input.keyboard.prev_key_state[key];
}

b32 is_mouse_button_down(int button) {
    return state.input.mouse.button_state[button];
}

b32 is_mouse_button_held(int button) {
    return state.input.mouse.button_state[button] && state.input.mouse.prev_button_state[button];
}

void process_inputs(GLFWwindow *window) {
    if (is_key_down(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }
}
