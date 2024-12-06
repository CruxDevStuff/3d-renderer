#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "SDL.h"

SDL_Window *window; 
SDL_Renderer *renderer; 
bool running; 

bool create_window(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("renderer",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600,
                                          SDL_WINDOW_BORDERLESS);
    if (!window) {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer) {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    return true; 
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void update(void) {
    return;
}

int main(void) {
    running = create_window(); 

    while (running) {
        handle_input(); 
        update();
        render_window(); 
    }

    printf("sup bro, you got me compiled, thank you...:))"); 
    return 0;
}