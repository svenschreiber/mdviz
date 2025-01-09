#ifndef SHADER_H
#define SHADER_H

GLuint load_shader_program(char *vertex_shader_file_path, char *fragment_shader_file_path);
GLuint load_shader_program_with_geometry_shader(char *vertex_shader_file_path, char *fragment_shader_file_path, char *geometry_shader_file_path);
GLuint load_shader_from_file(char *file_path, GLenum shader_type);

void shader_use(GLuint shader);

void shader_set_b32(GLuint shader, char *name, b32 value);
void shader_set_s32(GLuint shader, char *name, s32 value);
void shader_set_f32(GLuint shader, char *name, f32 value);
void shader_set_f32_array(GLuint shader, char *name, s32 count, f32 *value);
void shader_set_vec2(GLuint shader, char *name, vec2 value);
void shader_set_vec3(GLuint shader, char *name, vec3 value);
void shader_set_vec4(GLuint shader, char *name, vec4 value);
void shader_set_mat4(GLuint shader, char *name, mat4 value);

#endif
