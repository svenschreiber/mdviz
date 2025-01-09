GLuint load_shader_from_file(char *file_path, GLenum shader_type) {
    GLuint shader = glCreateShader(shader_type);
    String file = read_entire_file(file_path);
    glShaderSource(shader, 1, (const GLchar **)&file.data, (GLint *)&file.size);
    glCompileShader(shader);
    free(file.data);

    GLint is_compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == GL_FALSE) {
        GLint max_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);
        GLchar *error_log = ALLOC(GLchar, max_length);
        glGetShaderInfoLog(shader, max_length, &max_length, error_log);

        printf("%s\n", error_log);
        free(error_log);
        glDeleteShader(shader);

        return 0;
    }

    printf("[SHADER] Successfully loaded %s\n", file_path);

    return shader;
}

GLuint load_shader_program(char *vertex_shader_file_path, char *fragment_shader_file_path) {
    GLuint vertex_shader = load_shader_from_file(vertex_shader_file_path, GL_VERTEX_SHADER);
    GLuint fragment_shader = load_shader_from_file(fragment_shader_file_path, GL_FRAGMENT_SHADER);
    
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLint is_linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
    if (is_linked == GL_FALSE) {
        GLint max_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);
        GLchar *error_log = ALLOC(GLchar, max_length);
        glGetProgramInfoLog(program, max_length, &max_length, error_log);

        printf("%s\n", error_log);
        free(error_log);
        glDeleteProgram(program);

        return 0;
    }
    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

GLuint load_shader_program_with_geometry_shader(char *vertex_shader_file_path, char *fragment_shader_file_path, char *geometry_shader_file_path) {
    GLuint vertex_shader = load_shader_from_file(vertex_shader_file_path, GL_VERTEX_SHADER);
    GLuint fragment_shader = load_shader_from_file(fragment_shader_file_path, GL_FRAGMENT_SHADER);
    GLuint geometry_shader = load_shader_from_file(geometry_shader_file_path, GL_GEOMETRY_SHADER);
    
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glAttachShader(program, geometry_shader);
    glLinkProgram(program);

    GLint is_linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
    if (is_linked == GL_FALSE) {
        GLint max_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);
        GLchar *error_log = ALLOC(GLchar, max_length);
        glGetProgramInfoLog(program, max_length, &max_length, error_log);

        printf("%s\n", error_log);
        free(error_log);
        glDeleteProgram(program);

        return 0;
    }
    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);
    glDetachShader(program, geometry_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteShader(geometry_shader);

    return program;
}

void shader_use(GLuint shader) {
    glUseProgram(shader);
}

void shader_set_b32(GLuint shader, char *name, b32 value) {
    glUniform1i(glGetUniformLocation(shader, name), value);
}

void shader_set_s32(GLuint shader, char *name, s32 value) {
    glUniform1i(glGetUniformLocation(shader, name), value);
}

void shader_set_f32(GLuint shader, char *name, f32 value) {
    glUniform1f(glGetUniformLocation(shader, name), value);
}

void shader_set_f32_array(GLuint shader, char *name, s32 count, f32 *value) {
    glUniform1fv(glGetUniformLocation(shader, name), count, value);
}

void shader_set_vec2(GLuint shader, char *name, vec2 value) {
    glUniform2fv(glGetUniformLocation(shader, name), 1, &value.data[0]);
}

void shader_set_vec3(GLuint shader, char *name, vec3 value) {
    glUniform3fv(glGetUniformLocation(shader, name), 1, &value.data[0]);
}

void shader_set_vec4(GLuint shader, char *name, vec4 value) {
    glUniform4fv(glGetUniformLocation(shader, name), 1, &value.data[0]);
}

void shader_set_mat4(GLuint shader, char *name, mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(shader, name), 1, GL_FALSE, &value.data[0][0]);
}
