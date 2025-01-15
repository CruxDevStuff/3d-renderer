#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "vector.h"
#include "stdlib.h"
#include "texture.h"

typedef struct {
    int a, b, c; 
    uv_t a_uv; 
    uv_t b_uv; 
    uv_t c_uv; 
    uint32_t color; 
} face_t; 

typedef struct {
    vec2_t projected_vertices[3];
    uv_t uv[3]; 
    vec2_t projected_normal; 
    uint32_t color; 
    float z_depth; 
    float light_intensity; 
} triangle_t; 

triangle_t simple_triangle;

typedef enum {
    SOLID_COLOR,
    TEXTURED
} fill_t; 

#include "display.h"

void fill_triangle(triangle_t triangle, uint32_t color); 
void draw_triangle(triangle_t triangle, uint32_t color);
void fill_flat_bottom_triangle(vec2_t vertex_0, vec2_t vertex_1, vec2_t vertex_2, uint32_t color);
void fill_flat_top_triangle(vec2_t vertex_0, vec2_t vertex_1, vec2_t vertex_2, uint32_t color); 
void draw_textured_triangle(triangle_t triangle, uint32_t* texture); 
#endif