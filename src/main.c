#include "main.h"

const int cube_side = 9; 
const int POINT_N = cube_side * cube_side * cube_side;
const int FOV_SCALE_FACTOR = 1000; 
const int FPS = 30;
const int frame_time = (1000 / FPS); 

triangle_t projected_triangles[12]; 
vec3_t camera_position = {.x=0, .y=0, .z=-5}; 
vec3_t rotation = {.x=0, .y=0, .z=0}; 

uint32_t frame_wait_time;
uint32_t previous_frame_time = 0; 

void setup(void) {
    frame_buffer = (uint32_t*)malloc((sizeof(uint32_t)) * (window_width * window_height)); 

    if (frame_buffer == NULL) {
        return;
    }

    frame_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height); 
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

void draw_line(vec2_t p0, vec2_t p1) {
    int delta_x = p1.x - p0.x; 
    int delta_y = p1.y - p0.y; 

    /*
    MATH:
    compute x and y step size and direction by dividing dx and dx by largest side(dx or dy).
    by dynamically choosing largest side/base you increase the precision of increments of the smaller side instead of always dividing y over x to get step size.
    this approach makes drawing steep lines more precise.
    */

    int largest_side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y); 
    // int side_length = abs(delta_x); // uncomment to see why dynamic side selection is better
    float inc_x = delta_x / (float)largest_side_length; 
    float inc_y = delta_y / (float)largest_side_length; 

    vec2_t current_point = p0;

    for (int i = 0; i < largest_side_length; i++) {
        draw_pixel(round(current_point.x), round(current_point.y), 0xFF00FF00); 
        current_point.x += inc_x; 
        current_point.y += inc_y; 
    }
    
    // printf("step size: x: %f, y:%f side length:%d\n", inc_x, inc_y, side_length); 
}

void render_window(void) {
    // draw_grid(); 

    for (int i = 0; i < 12; i++) {
        // draw the projected vertices
        draw_rectangle(projected_triangles[i].projected_vertices[0].x, projected_triangles[i].projected_vertices[0].y, 4, 4, 0xFFFF0000);
        draw_rectangle(projected_triangles[i].projected_vertices[1].x, projected_triangles[i].projected_vertices[1].y, 4, 4, 0xFFFF0000);
        draw_rectangle(projected_triangles[i].projected_vertices[2].x, projected_triangles[i].projected_vertices[2].y, 4, 4, 0xFFFF0000);
        // draw lines between projected vertices
        draw_line(projected_triangles[i].projected_vertices[0], projected_triangles[i].projected_vertices[1]); 
        draw_line(projected_triangles[i].projected_vertices[1], projected_triangles[i].projected_vertices[2]); 
        draw_line(projected_triangles[i].projected_vertices[2], projected_triangles[i].projected_vertices[0]); 
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
    int wait_time = frame_time - (SDL_GetTicks() - previous_frame_time); 
    frame_wait_time = wait_time; // just a global copy of local wait time, servers no purpose

    if (wait_time > 0 && wait_time <= frame_time) {
        SDL_Delay(wait_time); 
    }

    previous_frame_time = SDL_GetTicks(); 


    rotation.y += 0.01; 
    rotation.x += 0.01; 
    rotation.z += 0.01; 
    
    vec3_t p0 = {.x=0, .y=0, .z=0}; 
    vec3_t p1 = {.x=20, .y=20, .z=20}; 

    for (int i = 0; i < 12; i++) {
        vec3_t vertices[3];
        vertices[0] =  cube_vertices[cube_faces[i].a - 1]; 
        vertices[1] =  cube_vertices[cube_faces[i].b - 1]; 
        vertices[2] =  cube_vertices[cube_faces[i].c - 1]; 
         
        projected_triangles[i].projected_vertices[0] = get_projection_2d(get_rotated_point(vertices[0], rotation)); 
        projected_triangles[i].projected_vertices[1] = get_projection_2d(get_rotated_point(vertices[1], rotation)); 
        projected_triangles[i].projected_vertices[2] = get_projection_2d(get_rotated_point(vertices[2], rotation)); 
    }
}

int main(void) {
    running = create_window(); 

    setup(); 

    while (running) {
        handle_input(); 
        update();
        render_window(); 
        // printf("wait time: %d\n", frame_wait_time);
    }

    cleanup(); 

    printf("BYE BYE...:))"); 
    return 0;
}