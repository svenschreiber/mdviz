Sim_Geometry make_sim_geometry(Sim_Sequence *seq) {
    Sim_Geometry geom = {0};
    glGenVertexArrays(1, &geom.vao);
    glGenBuffers(1, &geom.vbo);
    glBindVertexArray(geom.vao);
    glBindBuffer(GL_ARRAY_BUFFER, geom.vbo);
    Sim_Step *step = &seq->steps[seq->selected];
    glBufferData(GL_ARRAY_BUFFER, step->count * sizeof(Particle), step->particles, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (GLvoid *)0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (GLvoid *)(3 * sizeof(f32)));
    return geom;
}

void update_geometry(Sim_Geometry *geom) {
    glBindVertexArray(geom->vao);
    glBindBuffer(GL_ARRAY_BUFFER, geom->vbo);
    Sim_Step *step = &state.seq.steps[state.seq.selected];
    glBufferSubData(GL_ARRAY_BUFFER, 0, step->count * sizeof(Particle), step->particles);
}

