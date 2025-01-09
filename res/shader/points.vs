#version 410 core

layout (location = 0) in vec3 v_pos;

out vec3 f_pos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    f_pos = v_pos;
    gl_Position = projection * view * model * vec4(v_pos, 1.0);
}
