#ifndef MESH_H
#define MESH_H
#include "vector.h" 
#include "triangle.h"
#include "stdlib.h"
#include "stdio.h"
#include "array.h"
#include "string.h"

vec3_t cube_vertices[8];
face_t cube_faces[12]; 

static vec3_t default_scale = {.x=1.0, .y=1.0, .z=1.0};
static vec3_t default_rotation = {.x=0.0, .y=0.0, .z=0.0};
static vec3_t default_translation = {.x=0.0, .y=0.0, .z=0.0};

typedef struct {
    face_t* faces;
    vec3_t* vertices; 
    vec3_t translation;
    vec3_t rotation;
    vec3_t scale; 
} mesh_t; 

mesh_t* load_obj_file(char *filename); 
mesh_t* load_cube_data(void); 
#endif 