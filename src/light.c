#include "light.h"
/* some light vectors 
    left to right 
    light_ray.x=1.0/2.0; 
    light_ray.y=(-1.0/sqrt(2)); 
    light_ray.z=1.0/2.0; 

    right side forward
    light_ray.x=1.0/sqrt(2.0); 
    light_ray.y=0; 
    light_ray.z=1.0/sqrt(2.0); 
*/ 

float get_face_light_intensity(global_light_t light, vec3_t face_normal) { 
    return -(get_dotproduct(light.dir, face_normal)); // face normal points towards screen, so flip the dot product.
}

uint32_t get_light_intensity_adjusted_color(uint32_t color, float intensity) {
    uint32_t a = 0xFF000000;
    uint32_t r = (0x00FF0000 & color) * intensity;
    uint32_t g = (0x0000FF00 & color) * intensity;
    uint32_t b = (0x000000FF & color) * intensity;
    uint32_t adjusted_color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF); 
    return adjusted_color; 
}
