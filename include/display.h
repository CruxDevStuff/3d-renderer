#ifndef DISPLAY_H
#define DISPLAY_H

#include "stdlib.h"
#include "stdbool.h"
#include "SDL.h"
#include "triangle.h"

extern SDL_Window *window; 
extern SDL_Renderer *renderer; 
extern uint32_t *frame_buffer; 
extern SDL_Texture *frame_buffer_texture; 

int window_height;
int window_width; 

bool create_window(void);
void clear_frame_buffer(uint32_t clear_color);
void draw_grid(void); 
void draw_pixel(int x, int y, uint32_t color); 
void draw_rectangle(int x, int y, int width, int height, uint32_t color);
void draw_line(vec2_t p0, vec2_t p1); 
void draw_traingle(triangle_t triangle); 
void update_renderer_texture(void);

#endif