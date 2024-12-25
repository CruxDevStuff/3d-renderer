#include "triangle.h"

void fill_triangle(triangle_t triangle, uint32_t color) {
    // sort vertices by y coordinate
    vec2_t vertex_0 = triangle.projected_vertices[0]; 
    vec2_t vertex_1 = triangle.projected_vertices[1]; 
    vec2_t vertex_2 = triangle.projected_vertices[2]; 
    vec2_t temp; 

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

    /*
    MATH
    calculate the x value of the point of intersection 
    between the line y=vertex1.y and the line between points vertex 0 and vertex 2.
    */  
    float mid_x = (((vertex_2.x - vertex_0.x) * (vertex_1.y - vertex_0.y)) / (vertex_2.y - vertex_0.y)) + vertex_0.x; 
    vec2_t mid_point = {.x=mid_x, .y=vertex_1.y}; 

    /*
        SPLIT TRIANGLE INTO LOWER AND UPPER TRIANGLES

        **UPPER T VERTICES**
        1. vertex_0, 
        2. vertex_1, 
        3. mid_point

        **LOWER T VERTICES**
        1. vertex_1, 
        2. mid_point
        3. vertex_2, 
    */ 

    // draw mid line that divides the triangle 
    // draw_line(vertex_1, mid_point, 0xFF00FF00); 

    /*
    MATH:
    FILL UPPER TRIANGLE
    1. fill from top to bottom
    2. vertex 0(highest y after sorting) as the origin for the lines of the upper triangle
    */
    float upper_dy = vertex_0.y - vertex_1.y; 
    int u_cur_y = fabsf(upper_dy); 
    int u_y_length = fabsf(upper_dy); 

    // line 1 constants
    int dx1 = vertex_0.x - vertex_1.x;
    float m1 = upper_dy / dx1; 

    // line 2 constants
    int dx2 =  vertex_0.x - mid_point.x;
    float m2 = upper_dy / dx2; 

    for (int i = 0; i < u_y_length; i++) {
        // compute relative x and translate to origin 
        float x1 = (u_cur_y / m1) + vertex_0.x; 
        float x2 = (u_cur_y / m2) + vertex_0.x ; 
        int y = u_cur_y + vertex_0.y; 

        vec2_t start = {.x=x1, .y=y}; 
        vec2_t end = {.x=x2, .y=y}; 

        u_cur_y -= 1; // start from y of vertex 0 and decrement to go down
        draw_line(start, end, color); 
    }

    /*
    MATH:
    FILL LOWER TRIANGLE
    1. fill from bottom to top
    2. vertex 2(lowest y after sorting) as the origin for the lines of the lower triangle
    */
    float lower_dy = mid_point.y - vertex_2.y; 
    int l_y_length = fabsf(lower_dy); 

    // constants for line 1
    int l_dx_1 = vertex_2.x - vertex_1.x; 
    float l_m1 = lower_dy / l_dx_1; 

    // constants for line 2 
    int l_dx_2 = vertex_2.x - mid_point.x; 
    float l_m2 = lower_dy / l_dx_2; 

    for (int i = 0; i < l_y_length; i++) {
        // compute relative x and translate to origin 
        float x1 = (i / l_m1) + vertex_2.x; 
        float x2 = (i / l_m2) + vertex_2.x; 
        int y = vertex_2.y - i;  // start from y of vertex 2 and subtract to go up

        vec2_t start = {.x=x1, .y=y}; 
        vec2_t end = {.x=x2, .y=y}; 

        draw_line(start, end, color); 
    }
}