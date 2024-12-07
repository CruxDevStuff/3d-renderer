#include "main.h"

const int cube_side = 9; 
const int POINT_N = cube_side * cube_side * cube_side;
vec3_t cube_points[POINT_N];
vec2_t projected_points[POINT_N]; 

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
    vec2_t point_2d = {.x=256 * vec3_point.x, .y=256* vec3_point.y}; 
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
    for (int i = 0; i < POINT_N; i++){
        vec2_t point_2d = get_projection_2d(cube_points[i]); 
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

    printf("sup bro, you got me compiled, thank you...:))"); 
    return 0;
}