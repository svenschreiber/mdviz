#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef double FP_TYPE;

// Class handles
typedef struct MD_ParticleContainer MD_ParticleContainer;
typedef struct MD_ReflectiveBoundary MD_ReflectiveBoundary;
typedef struct MD_VelocityStoermerVerlet MD_VelocityStoermerVerlet;

// Problem
MD_ParticleContainer *md_get_aos_problem(size_t n_x, size_t n_y, size_t n_z, FP_TYPE v_particles, FP_TYPE max_x, FP_TYPE min_x, FP_TYPE max_y, FP_TYPE min_y, FP_TYPE max_z, FP_TYPE min_z);

// ParticleContainer
void md_cont_destroy(MD_ParticleContainer *cont);
size_t md_cont_size(MD_ParticleContainer *cont);
void md_cont_flush_forces(MD_ParticleContainer *cont);
void md_cont_get_data(MD_ParticleContainer *cont, float *result);

// LJForce
void md_ljforce_apply(MD_ParticleContainer *cont);

// ReflectiveBoundary
MD_ReflectiveBoundary *md_refl_bound_create(FP_TYPE min, FP_TYPE max, int dim);
void md_refl_bound_destroy(MD_ReflectiveBoundary *refl_bound);
void md_refl_bound_apply_cont(MD_ReflectiveBoundary *refl_bound, MD_ParticleContainer *cont);

// VelocityStoermerVerlet
MD_VelocityStoermerVerlet *md_vel_stoer_verlet_create(FP_TYPE dt);
void md_vel_stoer_verlet_destroy(MD_VelocityStoermerVerlet *vel_stoer_verlet);
void md_vel_stoer_verlet_update_positions(MD_VelocityStoermerVerlet *vel_stoer_verlet, MD_ParticleContainer *cont);
void md_vel_stoer_verlet_update_velocities(MD_VelocityStoermerVerlet *vel_stoer_verlet, MD_ParticleContainer *cont);

#ifdef __cplusplus
}
#endif
