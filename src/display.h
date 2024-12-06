#ifndef DISPLAY_H
#define DISPLAY_H

#include "stdlib.h"
#include "stdbool.h"
#include "SDL.h"

SDL_Window *window; 
SDL_Renderer *renderer; 
uint32_t *frame_buffer; 
SDL_Texture *frame_buffer_texture; 

int window_height;
int window_width; 
bool create_window(void);
void setup(void);
void clear_frame_buffer(uint32_t clear_color);
void draw_grid(void); 
void draw_rectangle(int x, int y, int width, int height, uint32_t color);
void render_window(void); 
void update_renderer_texture(void);
void cleanup(void); 

#endif