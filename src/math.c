vec2 v2_add(vec2 lhs, vec2 rhs) {
    return vec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

vec2 v2_sub(vec2 lhs, vec2 rhs) {
    return vec2(lhs.x - rhs.x, lhs.y - rhs.y);
}

vec2 v2_mul_f32(vec2 v, f32 f) {
    return vec2(v.x * f, v.y * f);
}

vec3 v3_add(vec3 lhs, vec3 rhs) {
    return vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

vec3 v3_sub(vec3 lhs, vec3 rhs) {
    return vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

vec3 v3_mul_f32(vec3 v, f32 f) {
    return vec3(v.x * f, v.y * f, v.z * f);
}

f32 v3_len(vec3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3 v3_norm(vec3 v) {
    f32 l = v3_len(v);
    return vec3(v.x / l, v.y / l, v.z / l);
}

f32 v3_dot(vec3 lhs, vec3 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

vec3 v3_cross(vec3 lhs, vec3 rhs) {
    return (vec3){
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    };
}

mat3 m3_ident() {
    return (mat3) {
        {
            {1.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 1.0f}
        }
    };
}

mat3 m3_transpose(mat3 m) {
    mat3 result;
    for (s32 i = 0; i < 3; ++i) {
        result.cols[i].x = m.data[0][i];
        result.cols[i].y = m.data[1][i];
        result.cols[i].z = m.data[2][i];
    }

    return result;
}

mat3 m3_rotate(mat3 m, vec3 axis, f32 theta) {
    m.data[0][0] = cosf(theta) + axis.x * axis.x * (1.0f - cosf(theta));
    m.data[1][0] = axis.x * axis.y * (1.0f - cosf(theta)) - axis.z * sinf(theta);
    m.data[2][0] = axis.x * axis.z * (1.0f - cosf(theta)) + axis.y * sinf(theta);

    m.data[0][1] = axis.y * axis.x * (1.0f - cosf(theta)) + axis.z * sinf(theta);
    m.data[1][1] = cosf(theta) + axis.y * axis.y * (1.0f - cosf(theta));
    m.data[2][1] = axis.y * axis.z * (1.0f - cosf(theta)) - axis.x * sinf(theta);

    m.data[0][2] = axis.z * axis.x * (1.0f - cosf(theta)) - axis.y * sinf(theta);
    m.data[1][2] = axis.z * axis.y * (1.0f - cosf(theta)) + axis.x * sinf(theta);
    m.data[2][2] = cosf(theta) + axis.z * axis.z * (1.0f - cosf(theta));
    
    return m;
}

vec3 m3_mul_v3(mat3 m, vec3 v) {
    vec3 result;
    for (s32 y = 0; y < 3; ++y) {
        result.data[y] = (m.data[0][y] * v.data[0] +
                          m.data[1][y] * v.data[1] +
                          m.data[2][y] * v.data[2]);
    }
    return result;
}

mat4 m4_ident() {
    return (mat4) {
        {
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        }
    };
}

mat4 m4_mul(mat4 lhs, mat4 rhs) {
    mat4 result;

#ifdef MATH_USE_SSE
    
    for (s32 i = 0; i < 4; ++i) {
        __m128 left = rhs.cols[i].sse;
        result.cols[i].sse = _mm_mul_ps(_mm_shuffle_ps(left, left, _MM_SHUFFLE(0, 0, 0, 0)), lhs.cols[0].sse);
        result.cols[i].sse = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(left, left, _MM_SHUFFLE(1, 1, 1, 1)), lhs.cols[1].sse), result.cols[i].sse);
        result.cols[i].sse = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(left, left, _MM_SHUFFLE(2, 2, 2, 2)), lhs.cols[2].sse), result.cols[i].sse);
        result.cols[i].sse = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(left, left, _MM_SHUFFLE(3, 3, 3, 3)), lhs.cols[3].sse), result.cols[i].sse);
    }

#else

    for (s32 x = 0; x < 4; ++x) {
        for (s32 y = 0; y < 4; ++y) {
            result.data[x][y] = (lhs.data[0][y] * rhs.data[x][0] +
                                 lhs.data[1][y] * rhs.data[x][1] +
                                 lhs.data[2][y] * rhs.data[x][2] +
                                 lhs.data[3][y] * rhs.data[x][3]);
        }
    }

#endif

    return result;
}

mat4 m4_mul_f32(mat4 m, f32 f) {
#ifdef MATH_USE_SSE

    __m128 f_sse = _mm_set1_ps(f);
    m.cols[0].sse = _mm_mul_ps(m.cols[0].sse, f_sse);
    m.cols[1].sse = _mm_mul_ps(m.cols[1].sse, f_sse);
    m.cols[2].sse = _mm_mul_ps(m.cols[2].sse, f_sse);
    m.cols[3].sse = _mm_mul_ps(m.cols[3].sse, f_sse);

#else

    for (s32 x = 0; x < 4; ++x) {
        for (s32 y = 0; y < 4; ++y) {
            m.data[x][y] *= f;
        }
    }

#endif

    return m;
}

vec4 v4_add(vec4 lhs, vec4 rhs) {
    return vec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

vec4 v4_sub(vec4 lhs, vec4 rhs) {
    return vec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

vec4 v4_mul_f32(vec4 v, f32 f) {
    return vec4(v.x * f, v.y * f, v.z * f, v.w * f);
}

f32 v4_dot(vec4 lhs, vec4 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

f32 v4_len(vec4 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

vec4 m4_mul_v4(mat4 m, vec4 v) {
    vec4 result;
#ifdef MATH_USE_SSE

    result.sse = _mm_mul_ps(_mm_shuffle_ps(v.sse, v.sse, _MM_SHUFFLE(0, 0, 0, 0)), m.cols[0].sse);
    result.sse = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(v.sse, v.sse, _MM_SHUFFLE(1, 1, 1, 1)), m.cols[1].sse), result.sse);
    result.sse = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(v.sse, v.sse, _MM_SHUFFLE(2, 2, 2, 2)), m.cols[2].sse), result.sse);
    result.sse = _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(v.sse, v.sse, _MM_SHUFFLE(3, 3, 3, 3)), m.cols[3].sse), result.sse);

#else

    for (s32 y = 0; y < 4; ++y) {
        result.data[y] = (m.data[0][y] * v.data[0] +
                          m.data[1][y] * v.data[1] +
                          m.data[2][y] * v.data[2] +
                          m.data[3][y] * v.data[3]);
    }

#endif

    return result;
}

mat4 m4_scale(mat4 m, vec3 scale) {
    m.data[0][0] = scale.x;
    m.data[1][1] = scale.y;
    m.data[2][2] = scale.z;
    return m;
}

mat4 m4_translate(mat4 m, vec3 translation) {
    m.data[3][0] = translation.x;
    m.data[3][1] = translation.y;
    m.data[3][2] = translation.z;
    return m;
}

mat4 m4_rotate(mat4 m, vec3 axis, f32 theta) {
    m.data[0][0] = cosf(theta) + axis.x * axis.x * (1.0f - cosf(theta));
    m.data[1][0] = axis.x * axis.y * (1.0f - cosf(theta)) - axis.z * sinf(theta);
    m.data[2][0] = axis.x * axis.z * (1.0f - cosf(theta)) + axis.y * sinf(theta);

    m.data[0][1] = axis.y * axis.x * (1.0f - cosf(theta)) + axis.z * sinf(theta);
    m.data[1][1] = cosf(theta) + axis.y * axis.y * (1.0f - cosf(theta));
    m.data[2][1] = axis.y * axis.z * (1.0f - cosf(theta)) - axis.x * sinf(theta);

    m.data[0][2] = axis.z * axis.x * (1.0f - cosf(theta)) - axis.y * sinf(theta);
    m.data[1][2] = axis.z * axis.y * (1.0f - cosf(theta)) + axis.x * sinf(theta);
    m.data[2][2] = cosf(theta) + axis.z * axis.z * (1.0f - cosf(theta));
    
    return m;
}

mat4 _m4_look_at(vec3 f, vec3 s, vec3 u, vec3 pos) {
    mat4 result = {0};
    
    result.data[0][0] = s.x;
    result.data[0][1] = u.x;
    result.data[0][2] = -f.x;

    result.data[1][0] = s.y;
    result.data[1][1] = u.y;
    result.data[1][2] = -f.y;

    result.data[2][0] = s.z;
    result.data[2][1] = u.z;
    result.data[2][2] = -f.z;

    result.data[3][0] = -v3_dot(s, pos);
    result.data[3][1] = -v3_dot(u, pos);
    result.data[3][2] = v3_dot(f, pos);
    result.data[3][3] = 1.0f;

    return result;
}

// right-handed look-at matrix
mat4 m4_look_at(vec3 pos, vec3 target, vec3 up) {
    vec3 f = v3_norm(v3_sub(target, pos));
    vec3 s = v3_norm(v3_cross(f, up));
    vec3 u = v3_cross(s, f);
    
    return _m4_look_at(f, s, u, pos);
}

mat4 m4_look_at_inv(mat4 look_at) {
    mat4 result = {0};

    mat3 rot = {0};
    rot.cols[0] = look_at.cols[0].xyz;
    rot.cols[1] = look_at.cols[1].xyz;
    rot.cols[2] = look_at.cols[2].xyz;
    rot = m3_transpose(rot);

    result.cols[0] = vec4(.xyz = rot.cols[0], .w = 0.0f);
    result.cols[1] = vec4(.xyz = rot.cols[1], .w = 0.0f);
    result.cols[2] = vec4(.xyz = rot.cols[2], .w = 0.0f);
    result.cols[3] = v4_mul_f32(look_at.cols[3], -1.0f);

    result.data[3][0] = -1.0f * look_at.data[3][0] / (rot.data[0][0] + rot.data[0][1] + rot.data[0][2]);
    result.data[3][1] = -1.0f * look_at.data[3][1] / (rot.data[1][0] + rot.data[1][1] + rot.data[1][2]);
    result.data[3][2] = -1.0f * look_at.data[3][2] / (rot.data[2][0] + rot.data[2][1] + rot.data[2][2]);
    result.data[3][3] = 1.0f;

    return result;
}

mat4 m4_ortho(s32 left, s32 right, s32 bottom, s32 top, f32 near, f32 far) {
    mat4 result = m4_ident();
    result.data[0][0] = 2.0f / (f32)(right - left);
    result.data[3][0] = -((f32)(right + left) / (f32)(right - left));
    result.data[1][1] = 2.0f / (f32)(top - bottom);
    result.data[3][1] = -((f32)(top + bottom) / (f32)(top - bottom));
    result.data[2][2] = -2.0f / (far - near);
    result.data[3][2] = -((far + near) / (far - near));

    return result;
}

mat4 m4_ortho_inv(mat4 ortho) {
    mat4 result = {0};
    result.data[0][0] = 1.0f / ortho.data[0][0];
    result.data[1][1] = 1.0f / ortho.data[1][1];
    result.data[2][2] = 1.0f / ortho.data[2][2];
    result.data[3][3] = 1.0f;

    result.data[3][0] = -ortho.data[3][0] * result.data[0][0];
    result.data[3][1] = -ortho.data[3][1] * result.data[1][1];
    result.data[3][2] = -ortho.data[3][2] * result.data[2][2];

    return result;
}

// right-handed perspective matrix
mat4 m4_perspective(f32 fov, f32 aspect, f32 near, f32 far) {
    mat4 result = {0};
    f32 e = 1.0f / tanf(fov / 2.0f);
    result.data[0][0] = e / aspect;
    result.data[1][1] = e;
    result.data[2][2] = -(far + near) / (far - near);
    result.data[3][2] = -2.0f * far * near / (far - near);
    result.data[2][3] = -1.0f;
    return result;
}

mat4 m4_perspective_inv(mat4 perspective) {
    mat4 result = {0};
    result.data[0][0] = 1.0f / perspective.data[0][0];
    result.data[1][1] = 1.0f / perspective.data[1][1];

    result.data[2][3] = 1.0f / perspective.data[3][2];
    result.data[3][3] = perspective.data[2][2] * result.data[2][3];
    result.data[3][2] = perspective.data[2][3];

    return result;
}

mat4 m4_transpose(mat4 m) {
    mat4 result;
    for (s32 i = 0; i < 4; ++i) {
        result.cols[i].x = m.data[0][i];
        result.cols[i].y = m.data[1][i];
        result.cols[i].z = m.data[2][i];
        result.cols[i].w = m.data[3][i];
    }

    return result;
}

mat4 m4_inv(mat4 m) {
    vec3 c01 = v3_cross(m.cols[0].xyz, m.cols[1].xyz);
    vec3 c23 = v3_cross(m.cols[2].xyz, m.cols[3].xyz);
    vec3 b10 = v3_sub(v3_mul_f32(m.cols[0].xyz, m.cols[1].w), v3_mul_f32(m.cols[1].xyz, m.cols[0].w));
    vec3 b32 = v3_sub(v3_mul_f32(m.cols[2].xyz, m.cols[3].w), v3_mul_f32(m.cols[3].xyz, m.cols[2].w));

    f32 inv_determinant = 1.0f / (v3_dot(c01, b32) + v3_dot(c23, b10));
    c01 = v3_mul_f32(c01, inv_determinant);
    c23 = v3_mul_f32(c23, inv_determinant);
    b10 = v3_mul_f32(b10, inv_determinant);
    b32 = v3_mul_f32(b32, inv_determinant);

    mat4 result;
    result.cols[0] = vec4(.xyz = v3_add(v3_cross(m.cols[1].xyz, b32), v3_mul_f32(c23, m.cols[1].w)), -v3_dot(m.cols[1].xyz, c23));
    result.cols[1] = vec4(.xyz = v3_sub(v3_cross(b32, m.cols[0].xyz), v3_mul_f32(c23, m.cols[0].w)),  v3_dot(m.cols[0].xyz, c23));
    result.cols[2] = vec4(.xyz = v3_add(v3_cross(m.cols[3].xyz, b10), v3_mul_f32(c01, m.cols[3].w)), -v3_dot(m.cols[3].xyz, c01));
    result.cols[3] = vec4(.xyz = v3_sub(v3_cross(b10, m.cols[2].xyz), v3_mul_f32(c01, m.cols[2].w)),  v3_dot(m.cols[2].xyz, c01));

    return m4_transpose(result);
}

quat quat_ident() {
    return quat(0.0f, 0.0f, 0.0f, 1.0f);
}

f32 quat_len(quat q) {
    return sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}

quat quat_conjugate(quat q) {
    return quat(-q.x, -q.y, -q.z, q.w);
}

quat quat_inv(quat q) {
    return quat_div_f32(quat_conjugate(q), quat_dot(q, q));
}

quat quat_add(quat lhs, quat rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    lhs.w += rhs.w;

    return lhs;
}

quat quat_sub(quat lhs, quat rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    lhs.w -= rhs.w;

    return lhs;
}

quat quat_mul(quat lhs, quat rhs) {
    quat result;

    result.x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
    result.y = lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z;
    result.z = lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x;
    result.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;

    return result;
}

quat quat_mul_f32(quat q, f32 f) {
    q.x *= f;
    q.y *= f;
    q.z *= f;
    q.w *= f;

    return q;
}

quat quat_div_f32(quat q, f32 f) {
    q.x /= f;
    q.y /= f;
    q.z /= f;
    q.w /= f;

    return q;
}

f32 quat_dot(quat lhs, quat rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

quat quat_norm(quat q) {
    return quat_div_f32(q, quat_len(q));
}

mat4 quat_to_m4(quat q) {
    mat4 result;
    q = quat_norm(q);

    f32 xx = q.x * q.x;
    f32 yy = q.y * q.y;
    f32 zz = q.z * q.z;
    f32 xy = q.x * q.y;
    f32 xz = q.x * q.z;
    f32 xw = q.x * q.w;
    f32 yz = q.y * q.z;
    f32 yw = q.y * q.w;
    f32 zw = q.z * q.w;

    result.data[0][0] = 1.0f - 2.0f * (yy + zz);
    result.data[0][1] = 2.0f * (xy + zw);
    result.data[0][2] = 2.0f * (xz - yw);
    result.data[0][3] = 0.0f;

    result.data[1][0] = 2.0f * (xy - zw);
    result.data[1][1] = 1.0f - 2.0f * (xx + zz);
    result.data[1][2] = 2.0f * (yz + xw);
    result.data[1][3] = 0.0f;

    result.data[2][0] = 2.0f * (xz + yw);
    result.data[2][1] = 2.0f * (yz - xw);
    result.data[2][2] = 1.0f - 2.0f * (xx + yy);
    result.data[2][3] = 0.0f;

    result.data[3][0] = 0.0f;
    result.data[3][1] = 0.0f;
    result.data[3][2] = 0.0f;
    result.data[3][3] = 1.0f;

    return result;
}

quat quat_from_euler(vec3 v, f32 angle) {
    quat result;

    vec3 n = v3_norm(v);
    f32 sine_half_rot = sinf(angle / 2.0f);

    result.xyz = v3_mul_f32(n, sine_half_rot);
    result.w = cosf(angle / 2.0f);

    return result;
}

f32 to_radians(f32 deg) {
    return deg * M_PI32 / 180.0f;
}
