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

typedef struct {
    face_t* faces;
    vec3_t* vertices; 
    vec3_t rotation;
} mesh_t; 

mesh_t* load_obj_file(char *filename); 
mesh_t* load_cube_data(void); 
#endif 