#ifndef MATRIX_H
#define MATRIX_H
#include "vector.h"

typedef struct {
    float values[4][4]; 
} matrix_4_t; 

matrix_4_t get_identity_matrix_4(); 
matrix_4_t get_scale_matrix(vec3_t scale); 

#endif
