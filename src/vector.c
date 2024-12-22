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

float get_magnitude_vec2(vec2_t vec) {
    return sqrt((vec.x * vec.x) + (vec.y * vec.y)); 
}

float get_magnitude_vec3(vec3_t vec) {
    return sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z)); 
}

vec2_t add_vec2(vec2_t a, vec2_t b) {
    vec2_t result = {.x = a.x + b.x, .y=a.y + b.y};
    return result;
}

vec2_t sub_vec2(vec2_t a, vec2_t b) {
    vec2_t result = {.x = a.x - b.x, .y=a.y - b.y};
    return result;
}

vec3_t add_vec3(vec3_t a, vec3_t b) {
    vec3_t result = {.x = a.x + b.x, .y=a.y + b.y, .z=a.z + b.z}; 
    return result; 
}

vec3_t sub_vec3(vec3_t a, vec3_t b) {
    vec3_t result = {.x = a.x - b.x, .y=a.y - b.y, .z=a.z - b.z};
    return result; 
}

vec2_t mul_vec2(vec2_t vec, float scale) {
    vec2_t result = {.x=vec.x * scale, .y=vec.y * scale}; 
}

vec2_t div_vec2(vec2_t vec, float scale) {
    vec2_t result = {.x=vec.x / scale, .y=vec.y / scale}; 
}

vec3_t mul_vec3(vec3_t vec, float scale) {
    vec3_t result = {.x=vec.x * scale, .y=vec.y * scale, .z=vec.z * scale}; 
    return result; 
}

vec3_t div_vec3(vec3_t vec, float scale) {
    vec3_t result = {.x=vec.x / scale, .y=vec.y / scale, .z=vec.z / scale}; 
    return result;
}

vec3_t get_crossproduct(vec3_t a, vec3_t b) {
    vec3_t result = {
        .x = (a.y * b.z) - (a.z * b.y), 
        .y = (a.z * b.x) - (a.x * b.z), 
        .z = (a.x * b.y) - (a.y * b.x)
    }; 
    return result; 
}

void test_vector_ops(void) {
    // this function is a masterclass on how to write shitty tests

    vec3_t vector3 = {.x=5, .y=5, .z=5}; 
    vec3_t vector3_ = {.x=2.5, .y=2.5, .z=5}; 

    vec2_t vector2 = {.x=5, .y=5}; 
    vec2_t vector2_ = {.x=3, .y=3}; 

    printf("LENGTH vec3: %f, vec2: %f\n", get_magnitude_vec3(vector3), get_magnitude_vec2(vector2)); 
    // printf("ADD vec3: %f, vec2: %f", get_magnitude_vec3(vector3), get_magnitude_vec2(vector2)); 
    vec3_t result3 = add_vec3(vector3, vector3_); 
    printf("ADD vec3: x: %f y: %f, z: %f\n", result3.x, result3.y, result3.z); 
    result3 = sub_vec3(vector3, vector3_); 
    printf("SUB vec3: x: %f y: %f, z: %f\n", result3.x, result3.y, result3.z); 

    vec2_t result2 = add_vec2(vector2, vector2_); 
    printf("ADD vec2: x: %f y: %f\n", result2.x, result2.y); 
    result2 = sub_vec2(vector2, vector2_); 
    printf("SUB: x: %f y: %f\n", result2.x, result2.y); 

    vec3_t a = {.x=9, .y=3, .z=6}; 
    vec3_t b = {.x=8, .y=4, .z=1}; 

    result3 = get_crossproduct(a, b); 
    printf("CROSS PRODUCT vec3: x: %f y: %f, z: %f\n", result3.x, result3.y, result3.z); 


}