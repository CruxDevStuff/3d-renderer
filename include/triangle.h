#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "vector.h"

typedef struct {
    int a, b, c; 
} face_t; 

typedef struct {
    vec2_t projected_vertices[3];
    vec2_t projected_normal; 
} triangle_t; 

triangle_t simple_triangle;
// simple_triangle.projected_vertices[0].x = 300;   
// simple_triangle.projected_vertices[0].y = 100;   

// simple_triangle.projected_vertices[1].x = 50;   
// simple_triangle.projected_vertices[1].y = 400;   

// simple_triangle.projected_vertices[2].x = 500;   
// simple_triangle.projected_vertices[2].y = 700;   

#include "display.h"

void fill_triangle(triangle_t triangle, uint32_t color); 
#endif