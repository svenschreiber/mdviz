void sim_init() {
    state.sim.type = CONT_ParticleContainer;

    // Problem:
    state.sim.problem.n        = ivec3(10, 10, 10);
    state.sim.problem.vel      = 0.5f;
    state.sim.problem.bounds_x = vec2(-55.0f, 55.0f);
    state.sim.problem.bounds_y = vec2(-55.0f, 55.0f);
    state.sim.problem.bounds_z = vec2(-55.0f, 55.0f);
    state.sim.problem.changed  = true;

    // Reflective Boundaries
    state.sim.boundaries.x = vec2(-55.0f + 1e-3, 55.0f - 1e-3);
    state.sim.boundaries.y = vec2(-55.0f + 1e-3, 55.0f - 1e-3);
    state.sim.boundaries.z = vec2(-55.0f + 1e-3, 55.0f - 1e-3);
    state.sim.boundaries.changed = true;

    // Integrator
    state.sim.integrator.dt      = 0.1f;
    state.sim.integrator.changed = true;

    // Sim playback
    state.sim.steps_per_second = 30;
}

// Constructs a new Problem/Container if the container type changed
void sim_update_problem() {
    if (state.sim.cont) md_cont_destroy(state.sim.cont);

    switch (state.sim.type) {
        case CONT_ParticleContainer: {
            state.sim.cont = md_get_aos_problem(
                state.sim.problem.n.x, state.sim.problem.n.y, state.sim.problem.n.z, 
                state.sim.problem.vel,
                state.sim.problem.bounds_x.y, state.sim.problem.bounds_x.x, 
                state.sim.problem.bounds_y.y, state.sim.problem.bounds_y.x, 
                state.sim.problem.bounds_z.y, state.sim.problem.bounds_z.x
            );
        } break;

        case CONT_SoAContainer: {
            state.sim.cont = md_get_soa_problem(
                state.sim.problem.n.x, state.sim.problem.n.y, state.sim.problem.n.z, 
                state.sim.problem.vel,
                state.sim.problem.bounds_x.y, state.sim.problem.bounds_x.x, 
                state.sim.problem.bounds_y.y, state.sim.problem.bounds_y.x, 
                state.sim.problem.bounds_z.y, state.sim.problem.bounds_z.x
            );
        } break;

        case CONT_LinkedCellContainer: {
            state.sim.cont = md_get_lc_problem(
                state.sim.problem.n.x, state.sim.problem.n.y, state.sim.problem.n.z, 
                state.sim.problem.vel,
                state.sim.problem.bounds_x.x, state.sim.problem.bounds_y.x, state.sim.problem.bounds_z.x,
                10.0, 11, 11, 11
            );
        } break;

        case CONT_ParallelLCContainer: {
            state.sim.cont = md_get_plc_problem(
                state.sim.problem.n.x, state.sim.problem.n.y, state.sim.problem.n.z, 
                state.sim.problem.vel,
                state.sim.problem.bounds_x.x, state.sim.problem.bounds_y.x, state.sim.problem.bounds_z.x,
                10.0, 11, 11, 11
            );
        } break;
    }

    state.sim.problem.changed = false;
}

// Constructs new boundary objects
void sim_update_boundaries() {
    if (state.sim.x_boundary) md_refl_bound_destroy(state.sim.x_boundary);
    if (state.sim.y_boundary) md_refl_bound_destroy(state.sim.y_boundary);
    if (state.sim.z_boundary) md_refl_bound_destroy(state.sim.z_boundary);
    state.sim.x_boundary = md_refl_bound_create(state.sim.boundaries.x.x, state.sim.boundaries.x.y, 0);
    state.sim.y_boundary = md_refl_bound_create(state.sim.boundaries.y.x, state.sim.boundaries.y.y, 1);
    state.sim.z_boundary = md_refl_bound_create(state.sim.boundaries.z.x, state.sim.boundaries.z.y, 2);
    state.sim.boundaries.changed = false;
}

// Constructs a new velocity stoermer verlet integrator
void sim_update_integrator() {
    md_vel_stoer_verlet_destroy(state.sim.vel_stoer_verlet);
    state.sim.vel_stoer_verlet = md_vel_stoer_verlet_create(state.sim.integrator.dt);
    state.sim.integrator.changed = false;
}

// Determines the min and max velocity values of all particles
vec2 sim_get_vel_bounds() {
    vec2 bounds = vec2(FLT_MAX, -FLT_MAX);
    for (u32 i = 0; i < state.sim.geometry.n_particles; ++i) {
        f32 v = state.sim.geometry.data[i].v;
        if (v < bounds.x) bounds.x = v;
        if (v > bounds.y) bounds.y = v;
    }
    return bounds;
}

// Allocates VBO data buffer and sends the particle geometry to GPU
void sim_update_geometry() {
    u32 n_particles = md_cont_size(state.sim.cont);
    if (state.sim.geometry.n_particles != n_particles) {
        if (state.sim.geometry.data) free(state.sim.geometry.data);
        state.sim.geometry.data = ALLOC(Particle, n_particles);
        state.sim.geometry.n_particles = n_particles;
        alloc_vbo_data(n_particles);
    }
    md_cont_get_data(state.sim.cont, (f32 *)state.sim.geometry.data);
    update_vbo_data();
    state.sim.geometry.vel_bounds = sim_get_vel_bounds();
    state.sim.geometry.changed = false;
}

// Updates any simulation components if it was changed (e.g. through the UI)
void sim_check_for_param_changes() {
    if (state.sim.problem.changed) {
        sim_update_problem();
        md_ljforce_apply(state.sim.cont); // init forces
        state.sim.geometry.changed = true;
    }

    if (state.sim.boundaries.changed) {
        sim_update_boundaries();
    }

    if (state.sim.integrator.changed) {
        sim_update_integrator();
    }

    if (state.sim.geometry.changed) {
        sim_update_geometry();
    }
}

// Make a simulation step
void sim_step() {
    md_vel_stoer_verlet_update_positions(state.sim.vel_stoer_verlet, state.sim.cont);

    // apply boundaries
    md_refl_bound_apply_cont(state.sim.x_boundary, state.sim.cont);
    md_refl_bound_apply_cont(state.sim.y_boundary, state.sim.cont);
    md_refl_bound_apply_cont(state.sim.z_boundary, state.sim.cont);

    // flush forces
    md_cont_flush_forces(state.sim.cont);

    // compute new forces
    md_ljforce_apply(state.sim.cont);

    // update velocities
    md_vel_stoer_verlet_update_velocities(state.sim.vel_stoer_verlet, state.sim.cont);
}

