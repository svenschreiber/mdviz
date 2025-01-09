#ifndef INPUT_H
#define INPUT_H

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

vec2 get_mouse_pos();
vec2 get_mouse_delta();
b32 is_key_down(int key);
b32 is_key_held(int key);
b32 is_mouse_button_down(int button);
b32 is_mouse_button_held(int button);

void process_inputs(GLFWwindow *window);

#endif // INPUT_H
