#ifndef MATRIX_H
#define MATRIX_H
#include "vector.h"
#include "math.h"

typedef struct {
    float values[4][4]; 
} matrix4_t; 

matrix4_t get_identity_matrix_4(); 
matrix4_t get_scale_matrix(vec3_t s); 
matrix4_t get_translation_matrix(vec3_t t); 
vec4_t mul_matrix4_vec4(matrix4_t m, vec4_t v); 
matrix4_t get_rotation_matrix_x(float rotation); 
matrix4_t get_rotation_matrix_y(float rotation); 
matrix4_t get_rotation_matrix_z(float rotation); 
matrix4_t get_rotation_matrix(vec3_t rot); 
matrix4_t mul_matrix4_matrix4(matrix4_t a, matrix4_t b); 
#endif
