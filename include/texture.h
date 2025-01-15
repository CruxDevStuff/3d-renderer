#ifndef TEXTURE_H
#define TEXTURE_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct {
    float u;
    float v;
} uv_t;

extern int texture_width;
extern int texture_height;
extern uint32_t* main_mesh_texture;
extern const uint8_t REDBRICK_TEXTURE[];

#endif
