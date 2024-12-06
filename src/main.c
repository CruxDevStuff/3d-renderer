#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "SDL.h"

SDL_Window *window; 
SDL_Renderer *renderer; 
bool running; 
uint32_t *frame_buffer; 
SDL_Texture *frame_buffer_texture; 

int window_height = 600;
int window_width = 800; 

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

void clear_frame_buffer(uint32_t wipe_color) {
    for (int i=0; i < (window_height * window_width); i++) {
        frame_buffer[i] = wipe_color; 
    }
}

void draw_grid(void) {
    for (int row=0; row < window_height; row++) {
        for (int column=0; column < window_width; column++) {
            if (row % 10 == 0 || column % 10 == 0) {
                int pixel_idx = (row * window_width) + column; 
                frame_buffer[pixel_idx] = 0xFFFFFFFF;
            }
        }
    }
}

void draw_rectangle(int x, int y, int width, int height, uint32_t color) {
    for (int row=y; row < height+y; row++) {
        for (int column=x; column < width+x; column++) {
                int pixel_idx = (row * window_width) + column; 
                frame_buffer[pixel_idx] = 0xFFFFFFFF;
        }
    }
}

void update_renderer_texture(void) {
    SDL_UpdateTexture(frame_buffer_texture, NULL, frame_buffer, (int)(window_width * sizeof(uint32_t))); 
    SDL_RenderCopy(renderer, frame_buffer_texture, NULL, NULL); 
}

void render_window(void) {
    // draw_grid(); 
    draw_rectangle((window_width/2) - 100, (window_height/2) - 100, 200, 200, 0xFFFFFFFF);
    update_renderer_texture();
    clear_frame_buffer(0xFFFF0000); 
    SDL_RenderPresent(renderer);
}


void update(void) {
}

void cleanup() {
    free(frame_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(void) {
    running = create_window(); 

    setup(); 

    while (running) {
        handle_input(); 
        update();
        render_window(); 
    }

    cleanup(); 

    printf("sup bro, you got me compiled, thank you...:))"); 
    return 0;
}