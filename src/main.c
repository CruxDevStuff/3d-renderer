#include "main.h"

const int cube_side = 9; 
const int POINT_N = cube_side * cube_side * cube_side;
const int FOV_SCALE_FACTOR = 600; 
const int FPS = 100;
const int frame_time = (1000 / FPS); 

uint32_t frame_wait_time;
uint32_t previous_frame_time = 0; 
float delta_time; 
float delta_time_scale_factor; 
uint32_t wait_time; 

// data in this mesh will rendered every frame, initialized before the game loop
mesh_t main_mesh; 
// main dynamic array that holds all the triangles to draw in the render step
triangle_t* triangle_buffer = NULL; 
matrix4_t proj_m; 

vec3_t origin = {.x=0, .y=0, .z=0}; 
camera_t main_camera = {.look_at={.x=0, .y=0, .z=1}}; 
const vec3_t up = {.x=0, .y=1, .z=0}; 

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

    frame_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, window_width, window_height); 
    proj_m = get_perspective_proj_matrix(M_PI/3, aspect_ratio, 0.1, 100); 

    // initialize z buffer to max depth value(100)
    size_t z_buffer_size = (sizeof(double)) * (window_width * window_height);
    z_buffer = (double*)malloc(z_buffer_size); 
    for (int i = 0; i < (window_width * window_height); i++) {
        z_buffer[i] = 100.0; 
    }
    // translation is inverted, the model should move to the left when camera moves to right(+ve X)
    vec3_t pos = {.x=0, .y=0, .z=0}; 
    main_camera.position = pos; 

    z_buffer_n = window_height * window_width; 
    texture_buffer_n = texture_height * texture_width; 
}

void handle_input(void) {
    // Simple FPS camera movement using Arrow keys

    const uint8_t* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_UP]) {
        main_camera.position.z += 1.5 * delta_time_scale_factor; 
    }

    if (state[SDL_SCANCODE_DOWN]) {
        main_camera.position.z -= 1.5 * delta_time_scale_factor; 
    }

    // if (state[SDL_SCANCODE_LEFT]) {
    //     main_camera.position.x -= 1.5 * delta_time_scale_factor; 
    // }

    // if (state[SDL_SCANCODE_RIGHT]) {
    //     main_camera.position.x += 1.5 * delta_time_scale_factor; 
    // }

    // if (state[SDL_SCANCODE_A]) {
    //     main_camera.rotation.y -= 0.5 * delta_time_scale_factor; 
    // }

    // if (state[SDL_SCANCODE_D]) {
    //     main_camera.rotation.y += 0.5 * delta_time_scale_factor; 
    // }

    // update camera look vector
    // main_camera.look_at.x = main_camera.position.x; 
    // matrix4_t look_vector_rotation_matrix = get_rotation_matrix(main_camera.rotation); 
    // main_camera.look_at = get_vec3_from_homogeneous(mul_matrix4_vec4(look_vector_rotation_matrix, get_homogeneous_from_vec3(main_camera.look_at))); 

    main_camera.look_at = add_vec3(main_camera.look_at, main_camera.position); 

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
                case SDLK_t:
                    render_settings->TEXTURE_FACES = !(render_settings->TEXTURE_FACES); 
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

void clear_z_buffer() {
    for (int i = 0; i < (window_width * window_height); i++) {
        z_buffer[i] = 100.0; 
    }
}

void render(void) {
    int triangle_count = array_length(triangle_buffer); 

    // sort triangle by depth high to low
    qsort(triangle_buffer, triangle_count, sizeof(triangle_t), z_compare); 

    clear_z_buffer(); 

    for (int i = 0; i < triangle_count; i++) {
        triangle_t cur_triangle = triangle_buffer[i];
        uint32_t outline_draw_color = get_light_intensity_adjusted_color(cur_triangle.color, cur_triangle.light_intensity);
        uint32_t raster_color = get_light_intensity_adjusted_color(cur_triangle.color, cur_triangle.light_intensity); 
        fill_t shade_type = render_settings->TEXTURE_FACES ? TEXTURE : SOLID;

        // highlight wireframe if enabled        
        if (render_settings->DRAW_WIREFRAME) {
            outline_draw_color = wireframe_color; 
        }

        // draw and fill triangle
        if (shade_type == SOLID) {
            draw_triangle(cur_triangle, outline_draw_color); 
        }

        if (render_settings->COLOR_FACES) {
            fill_triangle(cur_triangle, raster_color, shade_type); 
        }


        // draw normal lines
        if (render_settings->DRAW_FACE_NORMALS) {
            draw_line(vec2_from_vec4(cur_triangle.projected_vertices[0]), cur_triangle.projected_normal, 1, 0xFF00FF00); 
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
    upng_free(png_texture); 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

matrix4_t get_camera_view_matrix(camera_t camera) {
    vec3_t look_vector = sub_vec3(camera.look_at, camera.position);

    vec3_t z = get_normalized_vector(look_vector); 
    vec3_t x = get_normalized_vector(get_crossproduct(up, z)); 
    vec3_t y = get_crossproduct(z, x); 

    matrix4_t view_matrix = {.values = {0}};
    view_matrix.values[0][0] = x.x; 
    view_matrix.values[0][1] = x.y; 
    view_matrix.values[0][2] = x.z; 
    view_matrix.values[0][3] = -(get_dotproduct(x, camera.position)); 

    view_matrix.values[1][0] = y.x; 
    view_matrix.values[1][1] = y.y; 
    view_matrix.values[1][2] = y.z; 
    view_matrix.values[1][3] = -(get_dotproduct(y, camera.position)); 

    view_matrix.values[2][0] = z.x; 
    view_matrix.values[2][1] = z.y; 
    view_matrix.values[2][2] = z.z; 
    view_matrix.values[2][3] = -(get_dotproduct(z, camera.position));

    view_matrix.values[3][3] = 1.0; 

    return view_matrix; 
}

void update_camera_pose(camera_t *camera, vec3_t velocity, vec3_t orientation) {
    // move camera in global frame with relative translation input(position)
    camera->position.x += velocity.x; 
    camera->position.y += velocity.y; 
    camera->position.z += velocity.z; 
}

void update(void) {
    delta_time = SDL_GetTicks() - previous_frame_time;

    wait_time = frame_time - delta_time; 
    frame_wait_time = wait_time; // just a global copy of local wait time, servers no purpose

    if (wait_time > 0 && wait_time <= frame_time) {
        SDL_Delay(wait_time); 
    }

    delta_time = SDL_GetTicks() - previous_frame_time;
    delta_time_scale_factor = delta_time / 1000.0; 
    previous_frame_time = SDL_GetTicks(); 

    triangle_buffer = NULL; 
    triangle_t _triangle; 
    int mesh_face_count = array_length(main_mesh.faces); 

    // main_mesh.rotation.y += 0.01; 
    // main_mesh.rotation.y = -(M_PI/2); 
    // main_mesh.rotation.x = M_PI/1;                       
    // main_mesh.rotation.x += 0.01;
    // main_mesh.rotation.z += 0.01; 

    main_mesh.translation.z = 5; 
    // look_at.z = 5;
    // main_camera.position.x += 1.5 * delta_time_scale_factor;
    // main_camera.position.z += 1.5 * delta_time_scale_factor; 
    // main_camera.position.y += 1.5 * delta_time_scale_factor;

    // matrix to transform(rotate + translate) the vertices in the global reference frame 
    matrix4_t mesh_transformation_matrix = get_transformation_matrix(main_mesh.scale, main_mesh.rotation, main_mesh.translation); 

    // matrix transform the vertices to given camera frame using a view matrix
    matrix4_t view_matrix = get_camera_view_matrix(main_camera);

    // combine both global and camera view matrices 
    matrix4_t final_transformation_matrix = mul_matrix4_matrix4(view_matrix, mesh_transformation_matrix);

    // array_push(triangle_buffer, simple_triangle); 
    // return; 
 
    for (int i = 0; i < mesh_face_count; i++) {
        vec3_t vertices[3];

        vertices[0] =  main_mesh.vertices[main_mesh.faces[i].a - 1];
        vertices[1] =  main_mesh.vertices[main_mesh.faces[i].b - 1];
        vertices[2] =  main_mesh.vertices[main_mesh.faces[i].c - 1]; 
        
        vertices[0] = get_vec3_from_homogeneous(mul_matrix4_vec4(final_transformation_matrix, get_homogeneous_from_vec3(vertices[0]))); 
        vertices[1] = get_vec3_from_homogeneous(mul_matrix4_vec4(final_transformation_matrix, get_homogeneous_from_vec3(vertices[1]))); 
        vertices[2] = get_vec3_from_homogeneous(mul_matrix4_vec4(final_transformation_matrix, get_homogeneous_from_vec3(vertices[2]))); 

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

        vec3_t face_to_cam_ray = get_normalized_vector(sub_vec3(origin, vertices[0])); 
        vec3_t face_normal = get_normalized_vector(get_crossproduct(ab, ac)); 

        float cam_face_align = get_dotproduct(face_to_cam_ray, face_normal); 
        float face_light_intensity = get_face_light_intensity(global_light, face_normal); // face normal points towards screen, so flip the dot product.

        if (cam_face_align < 0 && render_settings->ENABLE_BACKFACE_CULLING) {
            continue;
        }

        _triangle.projected_vertices[0] = get_perspective_projected_point(vertices[0], proj_m); 
        _triangle.projected_vertices[1] = get_perspective_projected_point(vertices[1], proj_m); 
        _triangle.projected_vertices[2] = get_perspective_projected_point(vertices[2], proj_m); 
        _triangle.projected_normal = vec2_from_vec4(get_perspective_projected_point(add_vec3(div_vec3(face_normal, 2), vertices[0]), proj_m)); 
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

    const char *model_file_path = argv[1]; 
    int path_len = strlen(model_file_path); 
    mesh_t *model = load_obj_file(model_file_path); 

    if (model != NULL) {
        main_mesh = *model;

        // NOTE: texture file should be .png with the same name and at the same location as the .obj file
        // main_mesh_texture = (uint32_t*)REDBRICK_TEXTURE; 
        // TODO: extract texture width and height from file meta deta
        char *texture_file_path = malloc(strlen(argv[1]) + 1);
        strcpy(texture_file_path, argv[1]); 
        char *extension_delimeter = strrchr(texture_file_path, '.')+1; 
        strncpy(extension_delimeter, "png", 3); 
        main_mesh_texture = load_texture_from_png(texture_file_path); 
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