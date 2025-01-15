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
    float a; 
    float b; 
    float c; 
} barycentric_weights_t;

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
    SOLID,
    TEXTURED
} fill_t; 

#include "display.h"

void fill_triangle(triangle_t triangle, uint32_t color, fill_t FILL_TYPE); 
void draw_triangle(triangle_t triangle, uint32_t color);
void fill_flat_bottom_triangle(vec2_t vertex_0, vec2_t vertex_1, vec2_t vertex_2, vec2_t*parent_vertices ,uv_t uv_0, uv_t uv_1, uv_t uv_2, uint32_t color, fill_t FILL_TYPE);
void fill_flat_top_triangle(vec2_t vertex_0, vec2_t vertex_1, vec2_t vertex_2, vec2_t*parent_vertices, uv_t uv_0, uv_t uv_1, uv_t uv_2, uint32_t color, fill_t FILL_TYPE);
void draw_textured_triangle(triangle_t triangle, uint32_t* texture); 
void paint_texture(vec2_t current_point, uint32_t*texture, vec2_t*parent_vertices, uv_t uv_0, uv_t uv_1, uv_t uv_2);
barycentric_weights_t get_barrycentric_weights(vec2_t p, vec2_t a, vec2_t b, vec2_t c); 
#endif