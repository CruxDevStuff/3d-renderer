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

matrix4_t get_scale_matrix(vec3_t s) {
    matrix4_t sm = get_identity_matrix_4(); 
    sm.values[0][0] = s.x; 
    sm.values[1][1] = s.y; 
    sm.values[2][2] = s.z; 
    return sm; 
}

matrix4_t get_translation_matrix(vec3_t t) {
    matrix4_t tm = get_identity_matrix_4(); 
    tm.values[0][3] = t.x; 
    tm.values[1][3] = t.y; 
    tm.values[2][3] = t.z; 
    return tm; 
}

matrix4_t get_rotation_matrix_x(float rotation) {
    matrix4_t r = get_identity_matrix_4(); 
    r.values[1][1] = cos(rotation); 
    r.values[1][2] = -sin(rotation); 
    r.values[2][1] = sin(rotation); 
    r.values[2][2] = cos(rotation); 
    return r; 
}

matrix4_t get_rotation_matrix_y(float rotation) {
    matrix4_t r = get_identity_matrix_4(); 
    r.values[0][0] = cos(rotation); 
    r.values[0][2] = sin(rotation); 
    r.values[2][0] = -sin(rotation); 
    r.values[2][2] = cos(rotation); 
    return r; 
}

matrix4_t get_rotation_matrix_z(float rotation) {
    matrix4_t r = get_identity_matrix_4(); 
    r.values[0][0] = cos(rotation); 
    r.values[0][1] = -sin(rotation); 
    r.values[1][0] = sin(rotation); 
    r.values[1][1] = cos(rotation); 
    return r; 
}

vec4_t mul_matrix4_vec4(matrix4_t m, vec4_t v) {
    vec4_t ret = {
        .x=m.values[0][0] * v.x + m.values[0][1] * v.y + m.values[0][2] * v.z + m.values[0][3] * v.w, 
        .y=m.values[1][0] * v.x + m.values[1][1] * v.y + m.values[1][2] * v.z + m.values[1][3] * v.w, 
        .z=m.values[2][0] * v.x + m.values[2][1] * v.y + m.values[2][2] * v.z + m.values[2][3] * v.w, 
        .w=m.values[3][0] * v.x + m.values[3][1] * v.y + m.values[3][2] * v.z + m.values[3][3] * v.w 
    }; 
    return ret; 
}
