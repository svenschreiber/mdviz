#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef double FP_TYPE;

typedef struct MD_ParticleContainer MD_ParticleContainer;

MD_ParticleContainer *md_get_aos_problem(size_t n_x, size_t n_y, size_t n_z, FP_TYPE v_particles, FP_TYPE max_x, FP_TYPE min_x, FP_TYPE max_y, FP_TYPE min_y, FP_TYPE max_z, FP_TYPE min_z);
void md_cont_destroy(MD_ParticleContainer *cont);
size_t md_cont_size(MD_ParticleContainer *cont);

#ifdef __cplusplus
}
#endif
