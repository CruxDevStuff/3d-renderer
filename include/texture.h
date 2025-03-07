#ifndef TEXTURE_H
#define TEXTURE_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "upng.h"

typedef struct {
    float u;
    float v;
} uv_t;

extern int texture_width;
extern int texture_height;
extern uint32_t* main_mesh_texture;
extern upng_t* png_texture; 
extern const uint8_t REDBRICK_TEXTURE[];
uint32_t* load_texture_from_png(char* filename);
#endif
