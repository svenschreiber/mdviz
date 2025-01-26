#version 410 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in float v_vel;

out vec3 f_pos;
out vec4 f_col;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 cam_pos;
uniform vec2 vel_bounds;

void main()
{
    f_pos = v_pos;
    float alpha = 0.0;
    if (v_vel >= 0.5) {
        alpha = smoothstep(0.5, vel_bounds.y, v_vel);
        f_col = mix(vec4(1.0), vec4(1.0, 0.0, 0.0, 1.0), alpha);
    } else {
        alpha = smoothstep(0.5, vel_bounds.x, v_vel);
        f_col = mix(vec4(1.0), vec4(0.0, 0.0, 1.0, 1.0), alpha);
    }
    // alpha = smoothstep(vel_bounds.x, vel_bounds.y, v_vel);
    // f_col = mix(vec4(0.0, 0.0, 1.0, 1.0), vec4(1.0, 0.0, 0.0, 1.0), alpha);
    gl_PointSize = 5.0 * alpha + 4.0;
    gl_Position = projection * view * vec4(v_pos, 1.0);
}
