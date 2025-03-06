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
    matrix4_t r_x = get_rotation_matrix_x(rotation.x); 
    matrix4_t r_y = get_rotation_matrix_y(rotation.y); 
    matrix4_t r_z = get_rotation_matrix_z(rotation.z); 
    vec3_t v = get_vec3_from_homogeneous(mul_matrix4_vec4(r_x, get_homogeneous_from_vec3(point))); 
    v = get_vec3_from_homogeneous(mul_matrix4_vec4(r_y, get_homogeneous_from_vec3(v))); 
    v = get_vec3_from_homogeneous(mul_matrix4_vec4(r_z, get_homogeneous_from_vec3(v))); 
    return v; 
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
    return result; 
}

vec2_t div_vec2(vec2_t vec, float scale) {
    vec2_t result = {.x=vec.x / scale, .y=vec.y / scale}; 
    return result; 
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

float get_dotproduct(vec3_t a, vec3_t b) {
    return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z)); 
}

vec3_t get_normalized_vector(vec3_t vec) {
    float mag = get_magnitude_vec3(vec); 
    vec3_t normalized = {.x=vec.x/mag, .y=vec.y/mag, .z=vec.z/mag};
    return normalized; 
}

vec4_t get_homogeneous_from_vec3(vec3_t vec) {
    vec4_t v = {.x=vec.x, .y=vec.y, .z=vec.z, .w=1}; 
    return v; 
}

vec3_t get_vec3_from_homogeneous(vec4_t vec) {
    vec3_t v = {.x=vec.x, .y=vec.y, .z=vec.z}; 
    return v; 
}

vec2_t vec2_from_vec4(vec4_t v) {
    vec2_t r = {
        .x = v.x, 
        .y = v.y
    }; 

    return r; 
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