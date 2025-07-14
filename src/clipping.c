#include "clipping.h"

plane_t frustum_planes[6]; 

void init_frustum_planes(float fov, float z_near, float z_far) {
    float cos_half_fov = cos(fov/2); 
    float sin_half_fov = sin(fov/2); 

    frustum_planes[PLANE_LEFT] = (plane_t) {
        .origin = (vec3_t){.x=0, .y=0, .z=0}, 
        .normal = (vec3_t){.x=cos_half_fov, .y=0, .z=sin_half_fov}
    }; 
    frustum_planes[PLANE_RIGHT] = (plane_t) {
        .origin = (vec3_t){.x=0, .y=0, .z=0}, 
        .normal = (vec3_t){.x=-cos_half_fov, .y=0, .z=sin_half_fov}
    }; 
    frustum_planes[PLANE_TOP] = (plane_t) {
        .origin = (vec3_t){.x=0, .y=0, .z=0}, 
        .normal = (vec3_t){.x=0, .y=-cos_half_fov, .z=sin_half_fov}
    }; 
    frustum_planes[PLANE_BOTTOM] = (plane_t) {
        .origin = (vec3_t){.x=0, .y=0, .z=0}, 
        .normal = (vec3_t){.x=0, .y=cos_half_fov, .z=sin_half_fov}
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
    vec3_t prev_vertex, cur_vertex, prev_to_cur_vertex_vector; 
    float cur_vertex_dotp_plane, prev_vertex_dotp_plane; 
    // prev_vertex = polygon.vertices[0]; 
    // curr_vertex = polygon.vertices[1]; 

    // iterate through vertex pairs 
    for (int i = 0; i <= polygon.vertex_count; i++) {
        int vertex_idx = i; 

        // wrap around to first vertex 
        if (i == polygon.vertex_count) {
            vertex_idx = 0; 
        }

        cur_vertex = polygon.vertices[vertex_idx]; 

        cur_vertex_dotp_plane = get_dotproduct(sub_vec3(cur_vertex, clip_plane.origin), clip_plane.normal); 

        if (cur_vertex_dotp_plane >= 0 && i != polygon.vertex_count) {
            // inside
            clipped_polygon.vertices[clipped_vertex_idx] = cur_vertex; 
            clipped_vertex_idx++; 
        }

        if (cur_vertex_dotp_plane * prev_vertex_dotp_plane < 0 && i != 0) {
            // calculate intersection
            float t = prev_vertex_dotp_plane / (prev_vertex_dotp_plane - cur_vertex_dotp_plane); 
            prev_to_cur_vertex_vector = sub_vec3(cur_vertex, prev_vertex); 
            
            vec3_t clipped_vertex = mul_vec3(prev_to_cur_vertex_vector, t); 
            clipped_vertex = add_vec3(clipped_vertex, prev_vertex); 
            clipped_polygon.vertices[clipped_vertex_idx] = clipped_vertex;
            clipped_vertex_idx++; 
        }

        prev_vertex = cur_vertex; 
        prev_vertex_dotp_plane = cur_vertex_dotp_plane; 
    } 

    clipped_polygon.vertex_count = clipped_vertex_idx; 

    printf("vertex count :%d\n", clipped_polygon.vertex_count); 
    return clipped_polygon; 
}

clipped_face_t get_clipped_face(const vec3_t vertices[3]) {
    // clip and create clipped polygon 
    polygon_t face_polygon = {
        .vertex_count = 3, 
        .vertices[0] = vertices[0], 
        .vertices[1] = vertices[1], 
        .vertices[2] = vertices[2], 
    }; 
    polygon_t clipped_polygon = get_clipped_polygon_against_plane(frustum_planes[PLANE_LEFT], face_polygon); 
    clipped_polygon = get_clipped_polygon_against_plane(frustum_planes[PLANE_RIGHT], clipped_polygon); 
    clipped_polygon = get_clipped_polygon_against_plane(frustum_planes[PLANE_TOP], clipped_polygon); 
    clipped_polygon = get_clipped_polygon_against_plane(frustum_planes[PLANE_BOTTOM], clipped_polygon); 
    clipped_polygon = get_clipped_polygon_against_plane(frustum_planes[PLANE_NEAR], clipped_polygon); 
    clipped_polygon = get_clipped_polygon_against_plane(frustum_planes[PLANE_FAR], clipped_polygon); 

    // break polygon into triangles / faces  
    clipped_face_t clipped_face;
    return clipped_face;
}
