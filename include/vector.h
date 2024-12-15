#ifndef VECTOR_H
#define VECTOR_H

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
#endif 