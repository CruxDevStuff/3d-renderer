#include "main.h"

const int cube_side = 9; 
const int POINT_N = cube_side * cube_side * cube_side;
const int FOV_SCALE_FACTOR = 1000; 

vec3_t cube_points[POINT_N];
vec2_t projected_points[POINT_N]; 
vec3_t camera_position = {.x=0, .y=0, .z=-5}; 
vec3_t rotation = {.x=0, .y=0, .z=0}; 

void setup(void) {
    frame_buffer = (uint32_t*)malloc((sizeof(uint32_t)) * (window_width * window_height)); 

    if (frame_buffer == NULL) {
        return;
    }

    frame_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height); 
}

void create_point_cloud(void) {
    int point_idx = 0; 
    for (float x=-1; x <= 1; x+=0.25) {
        for (float y=-1; y <= 1; y+=0.25) {
            for (float z=-1; z <= 1; z+=0.25) {
                vec3_t new_point = {.x=x, .y=y, .z=z};
                cube_points[point_idx] = new_point;
                point_idx += 1; 
            }
        }
    }
}

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
    return point_2d;
}

void render_window(void) {
    // draw_grid(); 
    // draw_rectangle((window_width/2) - 100, (window_height/2) - 100, 400, 200, 0xFF00FFFF);
    for (int i = 0; i < POINT_N; i++){
        draw_rectangle(projected_points[i].x + (window_width / 2), projected_points[i].y + (window_height / 2), 4, 4, 0xFF00FFFF);
    }

    update_renderer_texture();
    clear_frame_buffer(0xFF000000); 
    SDL_RenderPresent(renderer);
}

void cleanup(void) {
    free(frame_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void update(void) {
    // for (int i = 0; i < POINT_N; i++) {
    //     vec3_t rotation = {.x=1, .y=1, .z=(3.14/2)}; 
    //     vec3_t rotated_point = get_rotated_point(cube_points[i], rotation); 
    //     cube_points[i] = rotated_point; 
    // }
    rotation.y += 0.01; 
    rotation.x += 0.01; 
    rotation.z += 0.01; 
    
    for (int i = 0; i < POINT_N; i++){
        vec3_t rotated_point = get_rotated_point_z(cube_points[i], rotation); 
        rotated_point = get_rotated_point_y(rotated_point, rotation); 
        rotated_point = get_rotated_point_x(rotated_point, rotation);
        vec2_t point_2d = get_projection_2d(rotated_point); 
        projected_points[i] = point_2d;
    }
}

int main(void) {
    running = create_window(); 

    setup(); 
    create_point_cloud(); 

    while (running) {
        handle_input(); 
        update();
        render_window(); 
    }

    cleanup(); 

    printf("BYE BYE...:))"); 
    return 0;
}