#include "main.h"

const int cube_side = 9; 
const int POINT_N = cube_side * cube_side * cube_side;
const int FOV_SCALE_FACTOR = 300; 
const int FPS = 30;
const int frame_time = (1000 / FPS); 

vec3_t rotation = {.x=0, .y=0, .z=0}; 
mesh_t main_mesh; 
triangle_t* triangle_buffer = NULL; // main dynamic array that holds all the trianngles to draw in the render step
vec3_t camera_position = {.x=0, .y=0, .z=-5}; 

uint32_t frame_wait_time;
uint32_t previous_frame_time = 0; 

void setup(void) {
    frame_buffer = (uint32_t*)malloc((sizeof(uint32_t)) * (window_width * window_height)); 
    if (frame_buffer == NULL) {
        return;
    }

    frame_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height); 

    // load cube data, just for testing
    for (int i = 0; i < 12; i++) {
        array_push(mesh.faces, cube_faces[i]); 
    }
    for (int i = 0; i < 8; i++) {
        array_push(mesh.vertices, cube_vertices[i]); 
    } 
    main_mesh.rotation = rotation; 
}

mesh_t* load_obj_file(char *filename) {
    // TODO: allocate this on the heap
    mesh_t* mesh = (mesh_t*)malloc(sizeof(mesh_t)); 
    // mesh = NULL;
    mesh_t _mesh; 
    FILE* f_ptr;  

    f_ptr = fopen(filename, "r"); 
    char read_line[500]; 

    if (f_ptr == NULL) {
        printf("ERROR: FAILED TO LOAD FILE"); 
        return mesh; // return empty mesh
    }

    // read lines and parse lines
    while (fgets(read_line, sizeof(read_line), f_ptr) != NULL) {
        char* token = strtok(read_line, " ");
        int i = 0; 
        char **token_values[10]; 

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
            array_push(main_mesh.vertices, point); 

        } 
        // handle face data
        else if (read_line[0] == 'f') {
            face_t face; 
            // set vertex indices
            face.a = atoi(strtok(token_values[1], "/")); 
            face.b = atoi(strtok(token_values[2], "/")); 
            face.c = atoi(strtok(token_values[3], "/")); 
            array_push(main_mesh.faces, face); 
        }
    }

    fclose(f_ptr); 
    // *mesh = _mesh; 
    return mesh; 
}

void handle_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);
    
    switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE){
                running = false; 
            }
        default:
            break;
    }
}

vec2_t get_projection_2d(vec3_t vec3_point) {
    /* MATH: 
    find projected value of X and Y on screen 
    using a property of similar triangles(the ratios of their sides are equal) 
    */
    vec3_point.z -= camera_position.z;
    vec2_t point_2d = {.x=(FOV_SCALE_FACTOR * vec3_point.x / vec3_point.z), 
                    .y=(FOV_SCALE_FACTOR * vec3_point.y / vec3_point.z)}; 

    
    /* 
    screen origin (0, 0) is at top left while our 3D cartesian space origin (0,0,0) is middle of the screen, 
    translate all points to account for this offset between world and screen space
    */
    point_2d.x += window_width / 2; 
    point_2d.y += window_height / 2; 

    return point_2d;
}

void render(void) {
    int triangle_count = array_length(triangle_buffer); 
    for (int i = 0; i < triangle_count; i++) {
        draw_triangle(triangle_buffer[i]); 
    }

    array_free(triangle_buffer); 
    update_renderer_texture();
    clear_frame_buffer(0xFF000000); 
    SDL_RenderPresent(renderer);
}

void cleanup(void) {
    free(frame_buffer);
    free(triangle_buffer); 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void update(void) {
    int wait_time = frame_time - (SDL_GetTicks() - previous_frame_time); 
    frame_wait_time = wait_time; // just a global copy of local wait time, servers no purpose
    if (wait_time > 0 && wait_time <= frame_time) {
        SDL_Delay(wait_time); 
    }
    previous_frame_time = SDL_GetTicks(); 


    triangle_buffer = NULL; 
    triangle_t _triangle; 
    int mesh_face_count = array_length(main_mesh.faces); 

    main_mesh.rotation.y += 0.01; 
    // main_mesh.rotation.x += 0.01; 
    main_mesh.rotation.z = 3.14; 

    for (int i = 0; i < mesh_face_count; i++) {
        vec3_t vertices[3];
        vertices[0] =  main_mesh.vertices[main_mesh.faces[i].a - 1]; 
        vertices[1] =  main_mesh.vertices[main_mesh.faces[i].b - 1]; 
        vertices[2] =  main_mesh.vertices[main_mesh.faces[i].c - 1]; 

        _triangle.projected_vertices[0] = get_projection_2d(get_rotated_point(vertices[0], main_mesh.rotation)); 
        _triangle.projected_vertices[1] = get_projection_2d(get_rotated_point(vertices[1], main_mesh.rotation)); 
        _triangle.projected_vertices[2] = get_projection_2d(get_rotated_point(vertices[2], main_mesh.rotation)); 

        array_push(triangle_buffer, _triangle); 
    }
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        printf("NO PATH TO OBJ FILE... EXITING: argc: %d", argc); 
        cleanup(); 
        return 0; 
    }

    running = create_window(); 
    setup(); 
    load_obj_file(argv[1]); 
    // return 0; 
    while (running) {
        handle_input(); 
        update();
        render(); 
        // printf("wait time: %d\n", frame_wait_time);
    }

    cleanup(); 

    printf("BYE BYE...:))"); 
    return 0;
}