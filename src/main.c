#include "main.h"

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

void render_window(void) {
    // draw_grid(); 
    draw_rectangle((window_width/2) - 100, (window_height/2) - 100, 400, 200, 0xFF00FFFF);
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