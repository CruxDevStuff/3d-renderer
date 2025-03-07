#include "mesh.h"

vec3_t cube_vertices[8] = {
    {.x=-1, .y=-1, .z=-1}, 
    {.x=-1, .y= 1, .z=-1}, 
    {.x= 1, .y= 1, .z=-1}, 
    {.x= 1, .y=-1, .z=-1}, 
    {.x= 1, .y= 1, .z= 1}, 
    {.x= 1, .y=-1, .z= 1}, 
    {.x=-1, .y= 1, .z= 1}, 
    {.x=-1, .y=-1, .z= 1}
};

face_t cube_faces[12] = {
    // front
    { .a = 1, .b = 2, .c = 3, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFFFF0000},
    { .a = 1, .b = 3, .c = 4, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFFFF0000},
    // right                                                                                 
    { .a = 4, .b = 3, .c = 5, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFF00FF00},
    { .a = 4, .b = 5, .c = 6, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFF00FF00},
    // back                                                                                  
    { .a = 6, .b = 5, .c = 7, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFF0000FF},
    { .a = 6, .b = 7, .c = 8, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFF0000FF},
    // left                                                                                  
    { .a = 8, .b = 7, .c = 2, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF},
    { .a = 8, .b = 2, .c = 1, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF},
    // top                                                                                   
    { .a = 2, .b = 7, .c = 5, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFFFFFF00},
    { .a = 2, .b = 5, .c = 3, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFFFFFF00},
    // bottom                                                                                
    { .a = 6, .b = 8, .c = 1, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFF00FFFF},
    { .a = 6, .b = 1, .c = 4, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFF00FFFF}
};

mesh_t* load_obj_file(char *filename) {
    mesh_t* mesh = (mesh_t*)malloc(sizeof(mesh_t)); 
    FILE* f_ptr;  

    if (!strcmp(filename, "cube")) {
        mesh = load_cube_data(); 
    } 

    f_ptr = fopen(filename, "r"); 
    char read_line[500]; 

    if (f_ptr == NULL) {
        printf("ERROR: FAILED TO LOAD FILE"); 
        return mesh; // return empty mesh
    }

    // parse file. only vertices and face data for now
    while (fgets(read_line, sizeof(read_line), f_ptr) != NULL) {
        char *token = strtok(read_line, " ");
        char **token_values[10]; 

        int i = 0; 
        while (token != NULL) {
            token_values[i] = token; 
            token = strtok(NULL, " ");
            i += 1;
        }

        char *id = token_values[0]; 

        // handle vertex data
        if (!strcmp(id, "v")) {
            vec3_t point;
            point.x = atof(token_values[1]); 
            point.y = atof(token_values[2]); 
            point.z = atof(token_values[3]); 
            array_push(mesh->vertices, point); 
        } 
        // handle face data
        else if (!strcmp(id, "f")) {
            face_t face; 
            // set vertex indices
            face.a = atoi(strtok(token_values[1], "/")); 
            face.b = atoi(strtok(token_values[2], "/")); 
            face.c = atoi(strtok(token_values[3], "/")); 
            face.color = 0xFFFFFFFF; // set all faces to white for now
            array_push(mesh->faces, face); 
        }
    }

    mesh->scale = default_scale; 
    mesh->rotation = default_rotation; 
    mesh->translation = default_translation; 
    
    fclose(f_ptr); 
    return mesh; 
}

mesh_t *load_cube_data(void) {
    mesh_t *mesh = (mesh_t*)malloc(sizeof(mesh_t)); 
    for (int i=0; i < 12; i++) {
        array_push(mesh->faces, cube_faces[i]); 
    }

    for (int i=0; i < 8; i++) {
        array_push(mesh->vertices, cube_vertices[i]); 
    }

    mesh->scale = default_scale; 
    mesh->rotation = default_rotation; 
    mesh->translation = default_translation; 

    return mesh;     
}



