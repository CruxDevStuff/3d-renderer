#include "display.h"
#include "mesh.h"
#include "stdio.h"
#include "vector.h"
#include "array.h"
#include "string.h"
#include "matrix.h"
#include "upng.h"
#include "light.h"

bool running = false; 
void setup(void); 
void handle_input(void); 
void render(void); 
void cleanup(void); 
void update(void); 
void clear_z_buffer(); 
uint32_t wireframe_color = 0xFF00FF00;