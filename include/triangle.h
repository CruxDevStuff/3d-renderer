#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "vector.h"

typedef struct {
    int a, b, c; 
} face_t; 

typedef struct {
    vec2_t projected_vertices[3];
} triangle_t; 

#endif