#ifndef CLIPPING_H
#define CLIPPING_H

#include "triangle.h"

typedef struct {
    int face_count; 
    vec3_t vertices[10];
    face_t faces[10]; 
    /*
    10 for the array size is just arbitrary number to account for maximum number of possible new sub triangles which is 7. 
    MATH: 
    a triangle when clipped against all six planes turns into a 9 sided polygon(9 vertices), 
    number of triangles from a n sided polygon is n - 2. therefore 9 - 2 = 7. 
    10 is just a round number to acccount for maximum possible sub triangles which is 7.
    stack over flow explanation - https://math.stackexchange.com/questions/859454/maximum-number-of-vertices-in-intersection-of-triangle-with-box/
    */
} clipped_face_t; 

typedef struct {
    vec3_t origin; 
    vec3_t normal; 
} plane_t; 

typedef struct {
    int vertex_count; 
    vec3_t vertices[10]; 
    /*
    maximum possible new vertices for a triangle clipped against 6 planes is 9. 
    So round up to 10 for the vertices array size.
    */
} polygon_t; 

enum FRUSTUM_PLANES {
    PLANE_RIGHT, 
    PLANE_LEFT, 
    PLANE_NEAR, 
    PLANE_FAR, 
    PLANE_TOP, 
    PLANE_BOTTOM
}; 

polygon_t get_clipped_polygon_against_plane(plane_t clip_plane, polygon_t polygon);
clipped_face_t get_clipped_face(const vec3_t vertices[3]); 
void init_frustum_planes(float fov, float z_near, float z_far); 
#endif

