#include "display.h"

SDL_Window *window = NULL; 
SDL_Renderer *renderer = NULL; 
uint32_t *frame_buffer = NULL; 
SDL_Texture *frame_buffer_texture = NULL; 

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
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN); 

    // renderer 
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    return true; 
}

void clear_frame_buffer(uint32_t clear_color) {
    for (int i=0; i < (window_height * window_width); i++) {
        frame_buffer[i] = clear_color; 
    }
}

void draw_pixel(int x, int y, uint32_t color) {
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

void update_renderer_texture(void) {
    SDL_UpdateTexture(frame_buffer_texture, NULL, frame_buffer, (int)(window_width * sizeof(uint32_t))); 
    SDL_RenderCopy(renderer, frame_buffer_texture, NULL, NULL); 
}

