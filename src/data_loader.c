void update_velocity_bounds(Sim_Step *step, f32 v) {
    if (v < step->bounds.x) step->bounds.x = v;
    if (v > step->bounds.y) step->bounds.y = v;
}

Sim_Sequence load_sim_sequence(String csv) {
    // preprocessing
    csv = str_trim(csv);
    str_read_line(&csv, NULL);
    
    String *lines = str_split(csv, Str("\n"));
    
    // find step count by reading the last line
    String *cols = str_split(lines[arrlen(lines) - 1], Str(","));
    s32 step_count = str_to_s32(cols[0]) + 1;
    arrfree(cols);

    Sim_Sequence seq = {
        .steps = ALLOC(Sim_Step, step_count),
        .count = step_count,
        .selected = 0,
        .steps_per_second = 30,
    };
    u32 particles_per_step = arrlen(lines) / step_count;

    for (u32 step_idx = 0; step_idx < step_count; ++step_idx) {
        Sim_Step *step  = &seq.steps[step_idx];
        step->particles = ALLOC(Particle, particles_per_step);
        step->count     = particles_per_step;
        step->bounds    = vec2(FLT_MAX, -FLT_MAX);

        u32 start = step_idx * particles_per_step;
        u32 end   = (step_idx + 1) * particles_per_step;
        for (u32 i = start; i < end; ++i) {
            String line = lines[i];
            String *values = str_split(line, Str(","));
            assert(arrlen(values) == 5);
            step->particles[i - start] = (Particle){
                .x = str_to_f32(values[1]),
                .y = str_to_f32(values[2]),
                .z = str_to_f32(values[3]),
                .v = str_to_f32(values[4])
            };
            update_velocity_bounds(step, step->particles[i - start].v);
            arrfree(values);
        }
    }
    arrfree(lines);

    return seq;
}
