#ifndef MATH_H
#define MATH_H

#ifdef MATH_USE_SSE
#include <xmmintrin.h>
#endif

// +============+
// | DEFINTIONS |
// +============+

typedef union vec2 {
    struct {
        f32 x;
        f32 y;
    };
    f32 data[2];
} vec2;

typedef union vec3 {
    struct {
        f32 x;
        f32 y;
        f32 z;
    };

    struct {
        f32 r;
        f32 g;
        f32 b;
    };
    
    f32 data[3];
} vec3;

typedef union vec4 {
    struct {
        union {
            vec3 xyz;
            struct {
                f32 x;
                f32 y;
                f32 z;
            };
        };

        f32 w;
    };

    struct {
        union {
            vec3 rgb;
            struct {
                f32 r;
                f32 g;
                f32 b;
            };
        };

        f32 a;
    };
    
    f32 data[4];

#ifdef MATH_USE_SSE
    __m128 sse;
#endif

} vec4;

typedef union ivec2 {
    struct {
        s32 x;
        s32 y;
    };

    struct {
        s32 width;
        s32 height;
    };

    s32 data[2];
} ivec2;

typedef union ivec3 {
    struct {
        s32 x;
        s32 y;
        s32 z;
    };

    struct {
        s32 r;
        s32 g;
        s32 b;
    };
    
    s32 data[3];
} ivec3;

typedef union ivec4 {
    struct {
        s32 x;
        s32 y;
        s32 z;
        s32 w;
    };

    struct {
        s32 r;
        s32 g;
        s32 b;
        s32 a;
    };
    
    s32 data[4];
} ivec4;

typedef union mat3 {
    f32 data[3][3];

    vec3 cols[3];
} mat3;

typedef union mat4 {
    f32 data[4][4];

    vec4 cols[4];
} mat4;

typedef union quat {
    struct {
        union {
            vec3 xyz;
            struct {
                f32 x;
                f32 y;
                f32 z;
            };
        };

        f32 w;
    };

    f32 data[4];
} quat;

// +===========+
// | INTERFACE |
// +===========+

vec2 v2_add(vec2 lhs, vec2 rhs);
vec2 v2_sub(vec2 lhs, vec2 rhs);
vec2 v2_mul_f32(vec2 v, f32 f);

vec3 v3_add(vec3 lhs, vec3 rhs);
vec3 v3_sub(vec3 lhs, vec3 rhs);
vec3 v3_mul_f32(vec3 v, f32 f);
f32 v3_len(vec3 v);
vec3 v3_norm(vec3 v);
f32 v3_dot(vec3 lhs, vec3 rhs);
vec3 v3_cross(vec3 lhs, vec3 rhs);

vec4 v4_add(vec4 lhs, vec4 rhs);
vec4 v4_sub(vec4 lhs, vec4 rhs);
vec4 v4_mul_f32(vec4 v, f32 f);
f32 v4_dot(vec4 lhs, vec4 rhs);
f32 v4_len(vec4 v);

mat3 m3_ident();
mat3 m3_transpose(mat3 m);
mat3 m3_rotate(mat3 m, vec3 axis, f32 theta);
vec3 m3_mul_v3(mat3 m, vec3 v);

mat4 m4_ident();
mat4 m4_mul(mat4 lhs, mat4 rhs);
mat4 m4_mul_f32(mat4 m, f32 f);
vec4 m4_mul_v4(mat4 m, vec4 v);
mat4 m4_scale(mat4 m, vec3 scale);
mat4 m4_translate(mat4 m, vec3 translation);
mat4 m4_rotate(mat4 m, vec3 axis, f32 theta);
mat4 m4_look_at(vec3 pos, vec3 target, vec3 up);
mat4 m4_look_at_inv(mat4 look_at);
mat4 m4_ortho(s32 left, s32 right, s32 bottom, s32 top, f32 near, f32 far);
mat4 m4_ortho_inv(mat4 ortho);
mat4 m4_perspective(f32 fov, f32 aspect, f32 near, f32 far);
mat4 m4_perspective_inv(mat4 perspective);
mat4 m4_inv(mat4 m);
mat4 m4_transpose(mat4 m);

quat quat_ident();
f32 quat_len(quat q);
quat quat_conjugate(quat q); 
quat quat_inv(quat q);
quat quat_add(quat lhs, quat rhs);
quat quat_sub(quat lhs, quat rhs);
quat quat_mul(quat lhs, quat rhs);
quat quat_mul_f32(quat q, f32 f);
quat quat_div_f32(quat q, f32 f);
quat quat_diff(quat lhs, quat rhs);
f32 quat_dot(quat lhs, quat rhs);
quat quat_norm(quat q);
mat4 quat_to_m4(quat q);
quat quat_from_euler(vec3 v, f32 angle);

f32 to_radians(f32 deg);

// +===============+
// | HELPER MACROS |
// +===============+

#define M_PI32 3.14159274101257324219f
#define M_FLT_MAX 3.402823466e+38F
#define Clamp(a, x, b) (((a)>(x))?(a):((b)<(x))?(b):(x))

#define vec2(...) (vec2){ __VA_ARGS__ }
#define vec3(...) (vec3){ __VA_ARGS__ }
#define vec4(...) (vec4){ __VA_ARGS__ }
#define ivec2(...) (ivec2){ __VA_ARGS__ }
#define ivec3(...) (ivec3){ __VA_ARGS__ }
#define ivec4(...) (ivec4){ __VA_ARGS__ }
#define quat(...) (quat){ __VA_ARGS__ }

#endif // MATH_H
