#include "matrix.h"

matrix4_t get_identity_matrix_4() {
    matrix4_t I = {
        .values[0][0] = 1, .values[0][1] = 0, .values[0][2] = 0, .values[0][3] = 0,
        .values[1][0] = 0, .values[1][1] = 1, .values[1][2] = 0, .values[1][3] = 0, 
        .values[2][0] = 0, .values[2][1] = 0, .values[2][2] = 1, .values[2][3] = 0, 
        .values[3][0] = 0, .values[3][1] = 0, .values[3][2] = 0, .values[3][3] = 1
    }; 
    return I; 
}

matrix4_t get_scale_matrix(vec3_t scale) {
    matrix4_t s_m = get_identity_matrix_4(); 
    s_m.values[0][0] = scale.x; 
    s_m.values[1][1] = scale.y; 
    s_m.values[2][2] = scale.z; 
    return s_m; 
}

vec4_t matrix_4_vec_4_mul(matrix4_t m, vec4_t v) {
    vec4_t ret = {
        .x=m.values[0][0] * v.x + m.values[0][1] * v.y + m.values[0][2] * v.z + m.values[0][3] * v.w, 
        .y=m.values[1][0] * v.x + m.values[1][1] * v.y + m.values[1][2] * v.z + m.values[1][3] * v.w, 
        .z=m.values[2][0] * v.x + m.values[2][1] * v.y + m.values[2][2] * v.z + m.values[2][3] * v.w, 
        .w=m.values[3][0] * v.x + m.values[3][1] * v.y + m.values[3][2] * v.z + m.values[3][3] * v.w 
    }; 
    return ret; 
}
