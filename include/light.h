#ifndef LIGHT_H
#define LIGHT_H
#include "stdlib.h"
#include "vector.h"

typedef struct {
    vec3_t dir; 
} global_light_t;


uint32_t get_light_intensity_adjusted_color(uint32_t color, float intensity); 
float get_face_light_intensity(global_light_t light, vec3_t face_normal); 

#endif 