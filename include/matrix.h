#ifndef MATRIX_H
#define MATRIX_H
#include "vector.h"

typedef struct {
    float values[4][4]; 
} matrix4_t; 

matrix4_t get_identity_matrix_4(); 
matrix4_t get_scale_matrix(vec3_t scale); 
vec4_t matrix_4_vec_4_mul(matrix4_t m, vec4_t v); 
#endif
