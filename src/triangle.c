#include "triangle.h"

triangle_t simple_triangle = {
   .projected_vertices[0].x = 300,
   .projected_vertices[0].y = 100,

   .projected_vertices[1].x = 50,
   .projected_vertices[1].y = 400,

   .projected_vertices[2].x = 500,
   .projected_vertices[2].y = 700,
   .color=0xFFFFFFFF
};


void draw_triangle(triangle_t triangle, uint32_t color) {
    // draw vertices 
    if (render_settings->DRAW_VERTICES) {
        draw_rectangle(triangle.projected_vertices[0].x, triangle.projected_vertices[0].y, 4, 4, 0xFFFF0000);
        draw_rectangle(triangle.projected_vertices[1].x, triangle.projected_vertices[1].y, 4, 4, 0xFFFF0000);
        draw_rectangle(triangle.projected_vertices[2].x, triangle.projected_vertices[2].y, 4, 4, 0xFFFF0000);
    }

    // lines between vertices
    draw_line(triangle.projected_vertices[0], triangle.projected_vertices[1], 2, color); 
    draw_line(triangle.projected_vertices[1], triangle.projected_vertices[2], 2, color); 
    draw_line(triangle.projected_vertices[2], triangle.projected_vertices[0], 2, color); 
}

void fill_flat_bottom_triangle(vec2_t vertex_0, vec2_t vertex_1, vec2_t vertex_2, uint32_t color) {
    /*
    FILL UPPER TRIANGLE
    1. fill from top to bottom
    2. vertex 0(highest y after sorting) as the origin for the lines of the upper triangle
    */
    vec2_t start = {.x=vertex_0.x, .y=vertex_0.y}; 
    vec2_t end = {.x=vertex_0.x, .y=vertex_0.y}; 

    float u_dx_1 = vertex_0.x - vertex_1.x; 
    float u_dx_2 = vertex_2.x - vertex_0.x;
    float u_dy = fabsf(vertex_0.y - vertex_2.y); 
    float u_m1 = u_dx_1 / u_dy; 
    float u_m2 = u_dx_2 / u_dy; 

    for (int i = 0; i < (int)(u_dy); i++) {
        start.x -= u_m1; start.y += 1; 
        end.x += u_m2; end.y += 1;
        draw_line(start, end, 2, color); 
    }

}

void fill_flat_top_triangle(vec2_t vertex_0, vec2_t vertex_1, vec2_t vertex_2, uint32_t color) {
    /*
    FILL LOWER TRIANGLE
    1. fill from bottom to top
    2. vertex 2(lowest y after sorting) as the origin for the lines of the lower triangle
    */
    vec2_t start = {.x=vertex_2.x, .y=vertex_2.y}; 
    vec2_t end = {.x=vertex_2.x, .y=vertex_2.y}; 

    float l_dx_1 = vertex_2.x - vertex_1.x; 
    float l_dx_2 = vertex_0.x - vertex_2.x;
    float l_dy = fabsf(vertex_2.y - vertex_0.y); 
    float l_m1 = l_dx_1 / l_dy; 
    float l_m2 = l_dx_2 / l_dy; 

    for (int i = 0; i < (int)(l_dy); i++) {
        start.x -= l_m1; start.y -= 1; 
        end.x += l_m2; end.y -= 1;
        draw_line(start, end, 2, color); 
    }
}

void fill_triangle(triangle_t triangle, uint32_t color) {
    // sort vertices by y coordinate
    vec2_t vertex_0 = triangle.projected_vertices[0]; 
    vec2_t vertex_1 = triangle.projected_vertices[1]; 
    vec2_t vertex_2 = triangle.projected_vertices[2]; 
    vec2_t temp; 

    // sort vertices by y (hight to low)
    if (vertex_0.y > vertex_1.y) {
        temp = vertex_0; 
        vertex_0 = vertex_1; 
        vertex_1 = temp; 
    }

    if (vertex_1.y > vertex_2.y) {
        temp = vertex_1; 
        vertex_1 = vertex_2; 
        vertex_2 = temp; 
    }

    if (vertex_0.y > vertex_1.y) {
        temp = vertex_0; 
        vertex_0 = vertex_1; 
        vertex_1 = temp; 
    }
     
    if (vertex_1.y == vertex_2.y) {
        fill_flat_bottom_triangle(vertex_0, vertex_1, vertex_2, color); 
    } else if (vertex_0.y == vertex_1.y) {
        fill_flat_top_triangle(vertex_0, vertex_1, vertex_2, color); 
    } else {


    /*
    MATH
    calculate the x value of the point of intersection 
    between the line y=vertex1.y and the line between points vertex 0 and vertex 2.
    */  
    float mid_x = (((vertex_2.x - vertex_0.x) * (vertex_1.y - vertex_0.y)) / (vertex_2.y - vertex_0.y)) + vertex_0.x; 
    vec2_t mid_point = {.x=mid_x, .y=vertex_1.y}; 

    /*
        SPLIT TRIANGLE INTO LOWER AND UPPER TRIANGLES AND FILL EACH SEPARATELY 

        **UPPER T VERTICES**    **LOWER T VERTICES**
        1. vertex_0             1. vertex_1
        2. vertex_1             2. mid_point
        3. mid_point            3. vertex_2
    */ 

        fill_flat_bottom_triangle(vertex_0, vertex_1, mid_point, color); 
        fill_flat_top_triangle(vertex_1, mid_point, vertex_2, color); 
    }
}