#include "main.h"

const int cube_side = 9; 
const int POINT_N = cube_side * cube_side * cube_side;
const int FOV_SCALE_FACTOR = 600; 
const int FPS = 30;
const int frame_time = (1000 / FPS); 

vec3_t rotation = {.x=0, .y=0, .z=0}; 
vec3_t camera_position = {.x=0, .y=0, .z=0}; 
vec3_t origin = {.x=0, .y=0, .z=0}; 
uint32_t frame_wait_time;
uint32_t previous_frame_time = 0; 

// data in this mesh will rendered every frame, initialized before the game loop
mesh_t main_mesh; 
// main dynamic array that holds all the triangles to draw in the render step
triangle_t* triangle_buffer = NULL; 
matrix4_t proj_m; 

// light from middle of the screen
global_light_t global_light = {
    .dir.x = 0, 
    .dir.y = 0, 
    .dir.z = 1, 
}; 

void setup(void) {
    frame_buffer = (uint32_t*)malloc((sizeof(uint32_t)) * (window_width * window_height)); 
    if (frame_buffer == NULL) {
        return;
    }

    frame_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height); 
    proj_m = get_perspective_proj_matrix(M_PI/3, aspect_ratio, 0.05, 100); 
    main_mesh_texture = (uint32_t*)REDBRICK_TEXTURE; 
}

void handle_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);
    
    switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    running = false; 
                    break;
                case SDLK_w:
                    render_settings->DRAW_WIREFRAME = !(render_settings->DRAW_WIREFRAME); 
                    render_settings->COLOR_FACES = !(render_settings->COLOR_FACES); 
                    break; 
                case SDLK_v:
                    render_settings->DRAW_VERTICES = !(render_settings->DRAW_VERTICES); 
                    break; 
                case SDLK_n:
                    render_settings->DRAW_FACE_NORMALS = !(render_settings->DRAW_FACE_NORMALS); 
                    break;
                case SDLK_b:
                    render_settings->ENABLE_BACKFACE_CULLING = !(render_settings->ENABLE_BACKFACE_CULLING); 
                    break;
                case SDLK_c:
                    render_settings->COLOR_FACES = !(render_settings->COLOR_FACES); 
                    render_settings->DRAW_WIREFRAME = !(render_settings->DRAW_WIREFRAME); 
                    break;
                default:
                    break;
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

int z_compare(const void* a, const void* b) {
    float a_depth = (*(triangle_t*)(a)).z_depth; 
    float b_depth = (*(triangle_t*)(b)).z_depth; 

    if (a_depth > b_depth) {; 
        return -1; 
    }

    if (a_depth < b_depth) {; 
        return 1; 
    }

    return 0; 
}

void render(void) {
    int triangle_count = array_length(triangle_buffer); 

    // sort triangle by depth high to low
    qsort(triangle_buffer, triangle_count, sizeof(triangle_t), z_compare); 

    for (int i = 0; i < triangle_count; i++) {
        triangle_t cur_triangle = triangle_buffer[i];
        uint32_t outline_draw_color = get_light_intensity_adjusted_color(cur_triangle.color, cur_triangle.light_intensity);
        uint32_t raster_color = get_light_intensity_adjusted_color(cur_triangle.color, cur_triangle.light_intensity); 

        // highlight wireframe if enabled        
        if (render_settings->DRAW_WIREFRAME) {
            outline_draw_color = wireframe_color; 
        }

        // draw and fill triangle
        if (render_settings->COLOR_FACES) {
            fill_triangle(cur_triangle, raster_color, SOLID); 
        }
        // draw_triangle(cur_triangle, outline_draw_color); 

        // draw normal lines
        if (render_settings->DRAW_FACE_NORMALS) {
            draw_line(cur_triangle.projected_vertices[0], cur_triangle.projected_normal, 1, 0xFF00FF00); 
        }
    }

    array_free(triangle_buffer); 
    update_renderer_texture();
    clear_frame_buffer(0xFF000000); 
    SDL_RenderPresent(renderer);

}

void cleanup(void) {
    free(frame_buffer);
    free(triangle_buffer); 
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

    triangle_buffer = NULL; 
    triangle_t _triangle; 
    int mesh_face_count = array_length(main_mesh.faces); 

    // main_mesh.rotation.y += 0.01; 
    // main_mesh.rotation.x = M_PI/1; 
    main_mesh.rotation.x += 0.01;
    // main_mesh.rotation.z += 0.01; 

    main_mesh.translation.z = 5; 

    matrix4_t transformation_matrix = get_transformation_matrix(main_mesh.scale, main_mesh.rotation, main_mesh.translation); 

    // array_push(triangle_buffer, simple_triangle); 
    // return; 
 
    for (int i = 0; i < mesh_face_count; i++) {
        vec3_t vertices[3];

        vertices[0] =  main_mesh.vertices[main_mesh.faces[i].a - 1];
        vertices[1] =  main_mesh.vertices[main_mesh.faces[i].b - 1];
        vertices[2] =  main_mesh.vertices[main_mesh.faces[i].c - 1]; 
        
        vertices[0] = get_vec3_from_homogeneous(mul_matrix4_vec4(transformation_matrix, get_homogeneous_from_vec3(vertices[0]))); 
        vertices[1] = get_vec3_from_homogeneous(mul_matrix4_vec4(transformation_matrix, get_homogeneous_from_vec3(vertices[1]))); 
        vertices[2] = get_vec3_from_homogeneous(mul_matrix4_vec4(transformation_matrix, get_homogeneous_from_vec3(vertices[2]))); 

        /*
        BACK FACE CULLING - ONLY RENDER FACES THAT ARE FACING THE CAMERA
        MATH:
        Find which faces are visible to the camera by computing 
        the scalar projection between the normal vector of a face and the 
        ray from the camera that intresets the face. Only render faces whose 
        scalar_projetion > 0. Only faces that are less than 90 degree relative to camera frame will satisfy the condition
        */
        vec3_t ab = sub_vec3(vertices[1], vertices[0]); 
        vec3_t ac = sub_vec3(vertices[2], vertices[0]); 

        vec3_t face_to_cam_ray = get_normalized_vector(sub_vec3(camera_position, vertices[0])); 
        vec3_t face_normal = get_normalized_vector(get_crossproduct(ab, ac)); 

        float cam_face_align = get_dotproduct(face_to_cam_ray, face_normal); 
        float face_light_intensity = get_face_light_intensity(global_light, face_normal); // face normal points towards screen, so flip the dot product.

        if (cam_face_align < 0 && render_settings->ENABLE_BACKFACE_CULLING) {
            continue;
        }

        _triangle.projected_vertices[0] = get_perspective_projected_point(vertices[0], proj_m); 
        _triangle.projected_vertices[1] = get_perspective_projected_point(vertices[1], proj_m); 
        _triangle.projected_vertices[2] = get_perspective_projected_point(vertices[2], proj_m); 
        _triangle.projected_normal = get_perspective_projected_point(add_vec3(div_vec3(face_normal, 2), vertices[0]), proj_m); 
        _triangle.light_intensity = face_light_intensity; 

        _triangle.uv[0] = main_mesh.faces[i].a_uv; 
        _triangle.uv[1] = main_mesh.faces[i].b_uv; 
        _triangle.uv[2] = main_mesh.faces[i].c_uv; 

        _triangle.z_depth = (vertices[0].z + vertices[1].z + vertices[2].z) / 3.0;  // set the z depth of the face to be the average of their z components
        _triangle.color = main_mesh.faces[i].color;
        array_push(triangle_buffer, _triangle); 
    }
}

int main(int argc, char* argv[]) {
    // test_vector_ops(); 

    if (argc <= 1) {
        printf("NO PATH TO OBJ FILE... EXITING: argc: %d", argc); 
        cleanup(); 
        return 0; 
    }

    running = create_window(); 
    setup(); 

    mesh_t *model = load_obj_file(argv[1]); 

    if (model != NULL) {
        main_mesh = *model;
    }

    while (running) {
        handle_input(); 
        update();
        render(); 
        // printf("wait time: %d\n", frame_wait_time);
    }

    cleanup(); 

    printf("BYE BYE...:))"); 
    return 0;
}