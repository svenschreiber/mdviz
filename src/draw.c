// Create VAO and VBO + specify vertex attribs
void init_sim_geometry() {
    GLuint *vao = &state.sim.geometry.vao;
    GLuint *vbo = &state.sim.geometry.vbo;
    glGenVertexArrays(1, vao);
    glGenBuffers(1, vbo);
    glBindVertexArray(*vao);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (GLvoid *)0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (GLvoid *)(3 * sizeof(f32)));
}

// Allocate buffer on GPU for particle data
void alloc_vbo_data(u32 n_particles) {
    glBindVertexArray(state.sim.geometry.vao);
    glBindBuffer(GL_ARRAY_BUFFER, state.sim.geometry.vbo);
    glBufferData(GL_ARRAY_BUFFER, n_particles * sizeof(Particle), NULL, GL_DYNAMIC_DRAW);
}

// Upload particle data to GPU
void update_vbo_data() {
    glBindVertexArray(state.sim.geometry.vao);
    glBindBuffer(GL_ARRAY_BUFFER, state.sim.geometry.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, state.sim.geometry.n_particles * sizeof(Particle), state.sim.geometry.data);
}
