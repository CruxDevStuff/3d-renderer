#ifndef MESH_H
#define MESH_H
#include "vector.h" 
#include "triangle.h"

vec3_t cube_vertices[8];
face_t cube_faces[12]; 

typedef struct {
    face_t* faces;
    vec3_t* vertices; 
    vec3_t rotation;
} mesh_t; 

extern mesh_t mesh; 
#endif 