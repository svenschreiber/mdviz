#ifndef SIM_H
#define SIM_H

void sim_init();
void sim_update_problem();
void sim_update_boundaries();
void sim_update_integrator();
vec2 sim_get_vel_bounds();
void sim_update_geometry();
void sim_check_for_param_changes();

#endif // SIM_H
