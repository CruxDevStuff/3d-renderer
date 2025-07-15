#include "clipping.h"

plane_t frustum_planes[6]; 

void init_frustum_planes(float fov, float z_near, float z_far) {
    // refer to this page for understanding FOV in 3D graphics(X and Y FOV are different) - https://en.wikipedia.org/wiki/Field_of_view_in_video_games
    float fov_y = fov; 
    float cos_half_fov_y = cos(fov_y / 2.0); 
    float sin_half_fov_y = sin(fov_y / 2.0); 

    float fov_x = atan(tan(fov_y / 2.0) * aspect_ratio_x) * 2.0; 
    float cos_half_fov_x = cos(fov_x / 2.0); 
    float sin_half_fov_x = sin(fov_x / 2.0); 

    frustum_planes[PLANE_LEFT] = (plane_t) {
        .origin = (vec3_t){.x=0, .y=0, .z=0}, 
        .normal = (vec3_t){.x=cos_half_fov_x, .y=0, .z=sin_half_fov_x}
    }; 
    frustum_planes[PLANE_RIGHT] = (plane_t) {
        .origin = (vec3_t){.x=0, .y=0, .z=0}, 
        .normal = (vec3_t){.x=-cos_half_fov_x, .y=0, .z=sin_half_fov_x}
    }; 
    frustum_planes[PLANE_TOP] = (plane_t) {
        .origin = (vec3_t){.x=0, .y=0, .z=0}, 
        .normal = (vec3_t){.x=0, .y=-cos_half_fov_y, .z=sin_half_fov_y}
    }; 
    frustum_planes[PLANE_BOTTOM] = (plane_t) {
        .origin = (vec3_t){.x=0, .y=0, .z=0}, 
        .normal = (vec3_t){.x=0, .y=cos_half_fov_y, .z=sin_half_fov_y}
    }; 
    frustum_planes[PLANE_FAR] = (plane_t) {
        .origin = (vec3_t){.x=0, .y=0, .z=z_far}, 
        .normal = (vec3_t){.x=0, .y=0, .z=-1}
    }; 
    frustum_planes[PLANE_NEAR] = (plane_t) {
        .origin = (vec3_t){.x=0, .y=0, .z=z_near}, 
        .normal = (vec3_t){.x=0, .y=0, .z=1}
    }; 
}

polygon_t get_clipped_polygon_against_plane(plane_t clip_plane, polygon_t polygon) {
    polygon_t clipped_polygon; 

    int clipped_vertex_idx = 0; 
    float cur_vertex_dotp_plane, prev_vertex_dotp_plane; 

    vec3_t prev_vertex, cur_vertex, prev_to_cur_vertex_vector; 
    uv_t prev_uv, cur_uv, prev_to_cur_uv_vector; 

    // iterate through vertex pairs 
    for (int i = 0; i <= polygon.vertex_count; i++) {
        int vertex_idx = i; 

        // wrap around to first vertex 
        if (i == polygon.vertex_count) {
            vertex_idx = 0; 
        }

        cur_vertex = polygon.vertices[vertex_idx]; 
        cur_uv = polygon.uv[vertex_idx]; 

        cur_vertex_dotp_plane = get_dotproduct(sub_vec3(cur_vertex, clip_plane.origin), clip_plane.normal); 

        if (cur_vertex_dotp_plane * prev_vertex_dotp_plane < 0 && i != 0) {
            /* 
            calculate intersection of vertex and texture(uv) coordinates 
            using linear interpolation and add to return polygon
            */

            float t = prev_vertex_dotp_plane / (prev_vertex_dotp_plane - cur_vertex_dotp_plane); 

            prev_to_cur_vertex_vector = sub_vec3(cur_vertex, prev_vertex); 
            prev_to_cur_uv_vector = sub_uv(cur_uv, prev_uv); 

            vec3_t clipped_vertex = mul_vec3(prev_to_cur_vertex_vector, t); 
            uv_t clipped_uv = mul_uv(prev_to_cur_uv_vector, t); 

            clipped_vertex = add_vec3(clipped_vertex, prev_vertex); 
            clipped_uv = add_uv(clipped_uv, prev_uv); 

            clipped_polygon.vertices[clipped_vertex_idx] = clipped_vertex;
            clipped_polygon.uv[clipped_vertex_idx] = clipped_uv;

            clipped_vertex_idx++; 
        }

        if (cur_vertex_dotp_plane >= 0 && i != polygon.vertex_count) {
            // add vertex and uv coordinates to return polygon 
            clipped_polygon.vertices[clipped_vertex_idx] = cur_vertex; 
            clipped_polygon.uv[clipped_vertex_idx] = cur_uv;
            clipped_vertex_idx++; 
        }

        prev_vertex = cur_vertex; 
        prev_uv = cur_uv; 

        prev_vertex_dotp_plane = cur_vertex_dotp_plane; 
    } 

    clipped_polygon.vertex_count = clipped_vertex_idx; 

    // printf("vertex count :%d\n", clipped_polygon.vertex_count); 
    return clipped_polygon; 
}

clipped_face_t get_clipped_face(const vec3_t vertices[3], const uv_t uv[3]) {
    // clip and create clipped polygon 
    polygon_t face_polygon = {
        .vertex_count = 3, 
        .vertices[0] = vertices[0], 
        .vertices[1] = vertices[1], 
        .vertices[2] = vertices[2], 

        .uv[0] = uv[0], 
        .uv[1] = uv[1], 
        .uv[2] = uv[2] 
    }; 

    polygon_t clipped_polygon = get_clipped_polygon_against_plane(frustum_planes[PLANE_LEFT], face_polygon); 
    clipped_polygon = get_clipped_polygon_against_plane(frustum_planes[PLANE_RIGHT], clipped_polygon); 
    clipped_polygon = get_clipped_polygon_against_plane(frustum_planes[PLANE_TOP], clipped_polygon); 
    clipped_polygon = get_clipped_polygon_against_plane(frustum_planes[PLANE_BOTTOM], clipped_polygon); 
    clipped_polygon = get_clipped_polygon_against_plane(frustum_planes[PLANE_NEAR], clipped_polygon); 
    clipped_polygon = get_clipped_polygon_against_plane(frustum_planes[PLANE_FAR], clipped_polygon); 

    // create triangles/faces from clipped polygon  
    // Triangle fan - https://en.wikipedia.org/wiki/Triangle_fan
    clipped_face_t clipped_face = {.face_count=0, .vertex_count=0};

    for (int i = 0; i < clipped_polygon.vertex_count; i++) {
        clipped_face.vertices[i] = clipped_polygon.vertices[i]; 
        clipped_face.vertex_count++; 
    }

    for (int i = 0; i < (clipped_face.vertex_count-2); i++) {
        uv_t a_uv = clipped_polygon.uv[0]; 
        uv_t b_uv = clipped_polygon.uv[i+1]; 
        uv_t c_uv = clipped_polygon.uv[i+2]; 

        face_t cur_face = {.a=0, .b=i+1, .c=i+2, .a_uv=a_uv, .b_uv=b_uv, .c_uv=c_uv}; 

        clipped_face.faces[i] = cur_face; 
        clipped_face.face_count++; 
    }
    
    return clipped_face;
}
