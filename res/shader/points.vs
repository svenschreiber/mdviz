#version 410 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_vel;

out vec3 f_pos;

uniform mat4 projection;
uniform mat4 view;
// uniform mat4 model;

void main()
{
    f_pos = v_pos;
    gl_Position = projection * view * vec4(v_pos, 1.0);
}
