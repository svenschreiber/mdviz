#ifndef DRAW_H
#define DRAW_H

Sim_Geometry make_sim_geometry(Sim_Sequence *seq);
void update_geometry(Sim_Geometry *geom);
void set_geometry(Sim_Geometry *geom, f32 *data, u32 n);

#endif // DRAW_H
