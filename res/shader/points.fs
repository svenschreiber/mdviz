#version 410 core

out vec4 frag_color;

in vec3 f_pos;
in vec4 f_col;

void main()
{
	frag_color = f_col;
}
