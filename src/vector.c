#include "vector.h"
#include "math.h"

/*
MATH behind rotation: 
double angle formulas of COS and SIN 
idea of rotating in 3D: lock the axis you are rotating around(dont change anything in that axis) 
and consider the remaining 2 axis as relative X and Y axis and do 2D rotation using a double angle formulas.
*/
vec3_t get_rotated_point_z(vec3_t point, vec3_t rotation) {
    vec3_t rotated_z_point = {
        .x=point.x * cos(rotation.z) - point.y * sin(rotation.z),
        .y=point.x * sin(rotation.z) + point.y * cos(rotation.z),
        .z=point.z
    }; 

    return rotated_z_point;
}

vec3_t get_rotated_point_y(vec3_t point, vec3_t rotation) {
    vec3_t rotated_y_point = {
        .x=point.x * cos(rotation.y) - point.z * sin(rotation.y),
        .z=point.x * sin(rotation.y) + point.z * cos(rotation.y),
        .y=point.y
    }; 

    return rotated_y_point;
}

vec3_t get_rotated_point_x(vec3_t point, vec3_t rotation) {
    vec3_t rotated_x_point = {
        .z=point.z * cos(rotation.x) - point.y * sin(rotation.x),
        .y=point.z * sin(rotation.x) + point.y * cos(rotation.x),
        .x=point.x
    }; 

    return rotated_x_point;
}

vec3_t get_rotated_point(vec3_t point, vec3_t rotation) {
    // apply rotation in all three axis 
    vec3_t rotated_point = get_rotated_point_z(point, rotation); 
    rotated_point = get_rotated_point_y(rotated_point, rotation); 
    rotated_point = get_rotated_point_x(rotated_point, rotation);
    return rotated_point; 
}
