#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "vector.h"
#include "stdlib.h"
typedef struct {
    int a, b, c; 
    uint32_t color; 
} face_t; 

typedef struct {
    vec2_t projected_vertices[3];
    vec2_t projected_normal; 
    uint32_t color; 
} triangle_t; 

triangle_t simple_triangle;

#include "display.h"

void fill_triangle(triangle_t triangle, uint32_t color); 
void draw_triangle(triangle_t triangle, uint32_t color);

#endif