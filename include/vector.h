#ifndef VECTOR_H
#define VECTOR_H
#include "stdio.h"

typedef struct {
    float x;
    float y; 
} vec2_t;

typedef struct {
    float x;
    float y; 
    float z; 
} vec3_t;

vec3_t get_rotated_point(vec3_t point, vec3_t rotation); 
vec3_t get_rotated_point_z(vec3_t point, vec3_t rotation); 
vec3_t get_rotated_point_y(vec3_t point, vec3_t rotation); 
vec3_t get_rotated_point_x(vec3_t point, vec3_t rotation); 

float get_magnitude_vec2(vec2_t vec); 
float get_magnitude_vec3(vec3_t vec); 

vec2_t add_vec2(vec2_t a, vec2_t b);
vec2_t sub_vec2(vec2_t a, vec2_t b);

vec3_t add_vec3(vec3_t a, vec3_t b);
vec3_t sub_vec3(vec3_t a, vec3_t b);

vec3_t div_vec3(vec3_t vec, float scale); 

vec3_t get_crossproduct(vec3_t a, vec3_t b); 
float get_dotproduct(vec3_t a, vec3_t b);

vec3_t get_normalized_vector(vec3_t vec);

void test_vector_ops(void); 
#endif 