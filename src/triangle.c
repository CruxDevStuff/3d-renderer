#include "triangle.h"

triangle_t simple_triangle = {
   .projected_vertices[0].x = 300,
   .projected_vertices[0].y = 100,

   .projected_vertices[1].x = 50,
   .projected_vertices[1].y = 400,

   .projected_vertices[2].x = 500,
   .projected_vertices[2].y = 700,
   .light_intensity = 1,
   .color=0xFFFFFFFF
};

void draw_triangle(triangle_t triangle, uint32_t color) {
    // draw vertices 
    if (render_settings->DRAW_VERTICES) {
        draw_rectangle(triangle.projected_vertices[0].x, triangle.projected_vertices[0].y, 4, 4, 0xFFFF0000);
        draw_rectangle(triangle.projected_vertices[1].x, triangle.projected_vertices[1].y, 4, 4, 0xFFFF0000);
        draw_rectangle(triangle.projected_vertices[2].x, triangle.projected_vertices[2].y, 4, 4, 0xFFFF0000);
    }

    // lines between vertices
    draw_line(vec2_from_vec4(triangle.projected_vertices[0]), vec2_from_vec4(triangle.projected_vertices[1]), 2, color); 
    draw_line(vec2_from_vec4(triangle.projected_vertices[1]), vec2_from_vec4(triangle.projected_vertices[2]), 2, color); 
    draw_line(vec2_from_vec4(triangle.projected_vertices[2]), vec2_from_vec4(triangle.projected_vertices[0]), 2, color); 
}

void fill_flat_bottom_triangle(vec4_t vertex_0, vec4_t vertex_1, vec4_t vertex_2, vec4_t*parent_vertices, uv_t uv_0, uv_t uv_1, uv_t uv_2, uint32_t color, fill_t FILL_TYPE) {
    /*
    FILL UPPER TRIANGLE
    1. fill from top to bottom
    2. vertex 0(highest y after sorting) as the origin for the lines of the upper triangle
    */
    vec4_t start = {.x=vertex_0.x, .y=vertex_0.y, .w=vertex_0.w}; 
    vec4_t end = {.x=vertex_0.x, .y=vertex_0.y, .w=vertex_0.w}; 

    float u_dx_1 = vertex_0.x - vertex_1.x; 
    float u_dx_2 = vertex_2.x - vertex_0.x;
    float u_dy = fabsf(vertex_0.y - vertex_2.y); 
    float u_m1 = u_dx_1 / u_dy; 
    float u_m2 = u_dx_2 / u_dy; 

    // barycentric_weights_t w; 
    for (int i = 0; i < (int)(u_dy); i++) {
        start.x -= u_m1; start.y += 1; 
        end.x += u_m2; end.y += 1;

        // interpolate in x(left to right or right to left) and color pixel; 
        if (start.x < end.x) {
            for (int x=start.x; x < end.x; x++) {
                vec4_t current_point = {.x=x, .y=start.y, .w=start.w};
                paint_texture(current_point, main_mesh_texture, parent_vertices, uv_0, uv_1, uv_2, FILL_TYPE, color); 
            }
        } else if (end.x < start.x) {
            for (int x=end.x; x < start.x; x++) {
                vec4_t current_point = {.x=x, .y=start.y, .w=start.w};
                paint_texture(current_point, main_mesh_texture, parent_vertices, uv_0, uv_1, uv_2, FILL_TYPE, color); 
                // draw_pixel(x, end.y, color); 
            }
        }

        // draw_line(start, end, 2, color); 
    }

}

void fill_flat_top_triangle(vec4_t vertex_0, vec4_t vertex_1, vec4_t vertex_2, vec4_t*parent_vertices, uv_t uv_0, uv_t uv_1, uv_t uv_2, uint32_t color, fill_t FILL_TYPE) {
    /*
    FILL LOWER TRIANGLE
    1. fill from bottom to top
    2. vertex 2(lowest y after sorting) as the origin for the lines of the lower triangle
    */
    vec2_t start = {.x=vertex_2.x, .y=vertex_2.y}; 
    vec2_t end = {.x=vertex_2.x, .y=vertex_2.y}; 

    float l_dx_1 = vertex_2.x - vertex_1.x; 
    float l_dx_2 = vertex_0.x - vertex_2.x;
    float l_dy = fabsf(vertex_2.y - vertex_0.y); 
    float l_m1 = l_dx_1 / l_dy; 
    float l_m2 = l_dx_2 / l_dy; 

    for (int i = 0; i < (int)(l_dy); i++) {
        start.x -= l_m1; start.y -= 1; 
        end.x += l_m2; end.y -= 1;
        
        // interpolate in x(left to right or right to left) and color pixel; 
        if (start.x < end.x) {
            for (int x=start.x; x < end.x; x++) {
                vec4_t current_point = {.x=x, .y=start.y};
                // TODO: check z buffer before draw call 
                paint_texture(current_point, main_mesh_texture, parent_vertices, uv_0, uv_1, uv_2, FILL_TYPE, color); 
            }
        } else if (end.x < start.x) {
            for (int x=end.x; x < start.x; x++) {
                vec4_t current_point = {.x=x, .y=start.y};
                paint_texture(current_point, main_mesh_texture, parent_vertices, uv_0, uv_1, uv_2, FILL_TYPE, color); 
            }
        }
    }
}

void fill_triangle(triangle_t triangle, uint32_t color, fill_t FILL_TYPE) {
    // sort vertices by y coordinate
    vec4_t vertex_0 = triangle.projected_vertices[0]; uv_t uv_0=triangle.uv[0]; 
    vec4_t vertex_1 = triangle.projected_vertices[1]; uv_t uv_1=triangle.uv[1]; 
    vec4_t vertex_2 = triangle.projected_vertices[2]; uv_t uv_2=triangle.uv[2]; 
    vec4_t temp_v; uv_t temp_uv; 

    // correct for flipped V coordinate because of different origin point interpretation in the obj file 
    uv_0.v = 1.0 - uv_0.v;
    uv_1.v = 1.0 - uv_1.v;
    uv_2.v = 1.0 - uv_2.v;

    // sort vertices by y (hight to low)
    if (vertex_0.y > vertex_1.y) {
        temp_v = vertex_0; 
        vertex_0 = vertex_1; 
        vertex_1 = temp_v; 

        temp_uv = uv_0; 
        uv_0 = uv_1; 
        uv_1 = temp_uv; 
    }

    if (vertex_1.y > vertex_2.y) {
        temp_v = vertex_1; 
        vertex_1 = vertex_2; 
        vertex_2 = temp_v; 
        
        temp_uv = uv_1; 
        uv_1 = uv_2; 
        uv_2 = temp_uv; 
    }

    if (vertex_0.y > vertex_1.y) {
        temp_v = vertex_0; 
        vertex_0 = vertex_1; 
        vertex_1 = temp_v; 

        temp_uv = uv_0; 
        uv_0 = uv_1; 
        uv_1 = temp_uv; 
    }
     
    vec4_t parent_vertices[3];
    parent_vertices[0] = vertex_0; 
    parent_vertices[1] = vertex_1; 
    parent_vertices[2] = vertex_2; 
    // barycentric_weights_t w = get_barrycentric_weights(vertex_0, vertex_0, vertex_1, vertex_2); 
    // printf("BARRY A: %f B: %f C: %f\n", w.a, w.b, w.c); 

    if (vertex_1.y == vertex_2.y) {
        fill_flat_bottom_triangle(vertex_0, vertex_1, vertex_2, parent_vertices, uv_0, uv_1, uv_2, color, FILL_TYPE); 
        return; 
    } else if (vertex_0.y == vertex_1.y) {
        fill_flat_top_triangle(vertex_0, vertex_1, vertex_2, parent_vertices, uv_0, uv_1, uv_2, color, FILL_TYPE); 
        return; 
    } 

    /*
    MATH
    calculate the x value of the point of intersection 
    between the line y=vertex1.y and the line between points vertex 0 and vertex 2.
    */  
    float mid_x = (((vertex_2.x - vertex_0.x) * (vertex_1.y - vertex_0.y)) / (vertex_2.y - vertex_0.y)) + vertex_0.x; 
    vec4_t mid_point = {.x=mid_x, .y=vertex_1.y}; 

    /*
        SPLIT TRIANGLE INTO LOWER AND UPPER TRIANGLES AND FILL EACH SEPARATELY 

        **UPPER T VERTICES**    **LOWER T VERTICES**
        1. vertex_0             1. vertex_1
        2. vertex_1             2. mid_point
        3. mid_point            3. vertex_2
    */ 

    fill_flat_bottom_triangle(vertex_0, vertex_1, mid_point, parent_vertices, uv_0, uv_1, uv_2, color, FILL_TYPE); 
    fill_flat_top_triangle(vertex_1, mid_point, vertex_2, parent_vertices, uv_0, uv_1, uv_2, color, FILL_TYPE); 
}

barycentric_weights_t get_barrycentric_weights(vec2_t p, vec2_t a, vec2_t b, vec2_t c) {
    // compute vectors(2D) of sides. for crossproduct purposes represent vec2 in vec3 with z set to 0.
    vec2_t ac = sub_vec2(c, a); 
    vec2_t ab = sub_vec2(b, a);  

    vec2_t pc = sub_vec2(c, p);  
    vec2_t pb = sub_vec2(b, p); 
    vec2_t ap = sub_vec2(p, a); 
    
    float _a = (pc.x * pb.y - pc.y * pb.x) / (ac.x * ab.y - ac.y * ab.x); 
    float _b = (ac.x * ap.y - ac.y * ap.x) / (ac.x * ab.y - ac.y * ab.x); 
    float _c = 1 - _a - _b; 

    barycentric_weights_t w = {
        .a = fabs(_a),
        .b = fabs(_b),
        .c = fabs(_c) 
    }; 

    return w; 
}


void paint_texture(vec4_t current_point, uint32_t*texture, vec4_t*parent_vertices, uv_t uv_0, uv_t uv_1, uv_t uv_2, fill_t fill_type, uint32_t solid_fill_color) {
    // get barycentric weights
    barycentric_weights_t w = get_barrycentric_weights(vec2_from_vec4(current_point), vec2_from_vec4(parent_vertices[0]), vec2_from_vec4(parent_vertices[1]), vec2_from_vec4(parent_vertices[2])); 

    double w_inverse = (1 / parent_vertices[0].w) * w.a + (1 / parent_vertices[1].w) * w.b + (1 / parent_vertices[2].w) * w.c;

    // z buffering. set the specfic pixel to the front most polygon's texture color 
    int z_idx = ((int)current_point.y * window_width) + (int)current_point.x; 
    double interpolated_z = 1 / w_inverse; 

    // prevent overflow / invalid access
    if (z_idx > (z_buffer_n - 1) || z_idx < 0) {
        return; 
    }

    if (interpolated_z <= z_buffer[z_idx]) {

        z_buffer[z_idx] = interpolated_z; 

        if (fill_type == TEXTURE) {
            // scale u with texture width and v with texture height to get the coordinates in texture frame
            uv_t scaled_uv = {
                .u = ((((uv_0.u / parent_vertices[0].w) * w.a + (uv_1.u / parent_vertices[1].w) * w.b + (uv_2.u / parent_vertices[2].w) * w.c)) * texture_width) / w_inverse,
                .v = ((((uv_0.v / parent_vertices[0].w) * w.a + (uv_1.v / parent_vertices[1].w) * w.b + (uv_2.v / parent_vertices[2].w) * w.c)) * texture_height) / w_inverse,
            }; 

            // convert from uv frame to 1D index to fetch correspoding color
            int index = ((int)scaled_uv.v * texture_width) + (int)scaled_uv.u; 
            
            // prevent overflow / invalid access
            if (index > (texture_buffer_n-1) || index < 0) {
                return; 
            }

            uint32_t color = texture[index]; 

            draw_pixel(current_point.x, current_point.y, color); 
        } 
        else if (fill_type == SOLID) {
            draw_pixel(current_point.x, current_point.y, solid_fill_color); 
        }
    }
}