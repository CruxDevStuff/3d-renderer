#include "display.h"

SDL_Window *window = NULL; 
SDL_Renderer *renderer = NULL; 
uint32_t *frame_buffer = NULL; 
SDL_Texture *frame_buffer_texture = NULL; 
render_settings_t *render_settings = NULL;

// FLAGS FOR RENDERING OPTIONS
bool DRAW_FACE_NOMRALS = false;  
bool DRAW_VERTICES = false;
bool DRAW_WIREFRAME = true; 
bool ENABLE_BACKFACE_CULLING = false; 

bool create_window(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    // window     
    SDL_DisplayMode display_node; 
    SDL_GetCurrentDisplayMode(0, &display_node); 
    window_width = display_node.w; 
    window_height = display_node.h; 
    
    window = SDL_CreateWindow("renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_BORDERLESS);
    if (!window) {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }
    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN); 

    // renderer 
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }
    render_settings = (render_settings_t*)malloc(sizeof(render_settings_t));

    // initial rendering state
    render_settings->DRAW_FACE_NORMALS = false; 
    render_settings->DRAW_VERTICES = false; 
    render_settings->ENABLE_BACKFACE_CULLING = true; 
    render_settings->DRAW_WIREFRAME = false; 
    render_settings->COLOR_FACES = true; 

    return true; 
}

void clear_frame_buffer(uint32_t clear_color) {
    for (int i=0; i < (window_height * window_width); i++) {
        frame_buffer[i] = clear_color; 
    }
}

void draw_pixel(int x, int y, uint32_t color) {
    if (x < 0 || x > window_width || y < 0 || y > window_height) {
        // printf("bad pixel access, X: %d, Y: %d\n", x, y); 
        return; 
    }

    frame_buffer[(y * window_width) + x] = color; 
}

void draw_grid(void) {
    for (int row=0; row < window_height; row++) {
        for (int column=0; column < window_width; column++) {
            if (row % 10 == 0 || column % 10 == 0) {
                draw_pixel(column, row, 0xFFFFFFFF);
            }
        }
    }
}

void draw_rectangle(int x, int y, int width, int height, uint32_t color) {
    for (int row=y; row < height+y; row++) {
        for (int column=x; column < width+x; column++) {
            draw_pixel(column, row, color);
        }
    }
}

void draw_line(vec2_t p0, vec2_t p1, int thickness, uint32_t color) {
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
        draw_rectangle(round(current_point.x), round(current_point.y), thickness, thickness, color);  // using draw_rectangle() instead of draw_pixel() here to draw THICCCer lines
        current_point.x += inc_x; 
        current_point.y += inc_y; 
    }
    
    // printf("step size: x: %f, y:%f side length:%d\n", inc_x, inc_y, side_length); 
}

void update_renderer_texture(void) {
    SDL_UpdateTexture(frame_buffer_texture, NULL, frame_buffer, (int)(window_width * sizeof(uint32_t))); 
    SDL_RenderCopy(renderer, frame_buffer_texture, NULL, NULL); 
}

