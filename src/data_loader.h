#ifndef DATA_LOADER_H
#define DATA_LOADER_H

typedef struct Particle {
    f32 x;
    f32 y;
    f32 z;
    f32 v;
} Particle;

typedef struct Sim_Step {
    Particle *particles;
    vec2 bounds;
    u32 count;
} Sim_Step;

typedef struct Sim_Sequence {
    Sim_Step *steps;
    u32 count;
    s32 selected;
    u32 steps_per_second;
    b32 playing;
    f32 timer;
} Sim_Sequence;

void update_velocity_bounds(Sim_Step *step, f32 v);
Sim_Sequence load_sim_sequence(String csv);


#endif // DATA_LOADER_H
