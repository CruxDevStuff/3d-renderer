#include "matrix.h"

matrix4_t get_identity_matrix_4() {
    matrix4_t I = {
        .values[0][0] = 1, .values[0][1] = 0, .values[0][2] = 0, .values[0][3] = 0,
        .values[1][0] = 0, .values[1][1] = 1, .values[1][2] = 0, .values[1][3] = 0, 
        .values[2][0] = 0, .values[2][1] = 0, .values[2][2] = 1, .values[2][3] = 0, 
        .values[3][0] = 0, .values[3][1] = 0, .values[3][2] = 0, .values[3][3] = 1
    }; 
    return I; 
}

matrix4_t get_scale_matrix(vec3_t s) {
    matrix4_t sm = get_identity_matrix_4(); 
    sm.values[0][0] = s.x; 
    sm.values[1][1] = s.y; 
    sm.values[2][2] = s.z; 
    return sm; 
}

matrix4_t get_translation_matrix(vec3_t t) {
    matrix4_t tm = get_identity_matrix_4(); 
    tm.values[0][3] = t.x; 
    tm.values[1][3] = t.y; 
    tm.values[2][3] = t.z; 
    return tm; 
}

matrix4_t get_rotation_matrix_x(float rotation) {
    matrix4_t r = get_identity_matrix_4(); 
    r.values[1][1] = cos(rotation); 
    r.values[1][2] = -sin(rotation); 
    r.values[2][1] = sin(rotation); 
    r.values[2][2] = cos(rotation); 
    return r; 
}

matrix4_t get_rotation_matrix_y(float rotation) {
    matrix4_t r = get_identity_matrix_4(); 
    r.values[0][0] = cos(rotation); 
    r.values[0][2] = sin(rotation); 
    r.values[2][0] = -sin(rotation); 
    r.values[2][2] = cos(rotation); 
    return r; 
}

matrix4_t get_rotation_matrix_z(float rotation) {
    matrix4_t r = get_identity_matrix_4(); 
    r.values[0][0] = cos(rotation); 
    r.values[0][1] = -sin(rotation); 
    r.values[1][0] = sin(rotation); 
    r.values[1][1] = cos(rotation); 
    return r; 
}

vec4_t mul_matrix4_vec4(matrix4_t m, vec4_t v) {
    vec4_t ret = {
        .x=m.values[0][0] * v.x + m.values[0][1] * v.y + m.values[0][2] * v.z + m.values[0][3] * v.w, 
        .y=m.values[1][0] * v.x + m.values[1][1] * v.y + m.values[1][2] * v.z + m.values[1][3] * v.w, 
        .z=m.values[2][0] * v.x + m.values[2][1] * v.y + m.values[2][2] * v.z + m.values[2][3] * v.w, 
        .w=m.values[3][0] * v.x + m.values[3][1] * v.y + m.values[3][2] * v.z + m.values[3][3] * v.w 
    }; 
    return ret; 
}

matrix4_t mul_matrix4_matrix4(matrix4_t a, matrix4_t b) {
    matrix4_t m; 
    for (int r = 0; r < 4; r++) {
        for (int i=0; i < 4; i++) {
            m.values[r][i] = a.values[r][0]*b.values[0][i] + a.values[r][1]*b.values[1][i] + a.values[r][2]*b.values[2][i]  + a.values[r][3]*b.values[3][i]; 
        }
    }
    return m;  
}

matrix4_t get_rotation_matrix(vec3_t rot) {
    // lefthand coordinate system, rotation in counterclock wise direction.
    // TODO: y axis spins in the wrong direction(clockwise), flipping it as a temporary hack. find out why. 
    float x = rot.x;  float y = rot.y; float z = -(rot.z); 
    matrix4_t r = get_identity_matrix_4(); 

    r.values[0][0] = cos(y)*cos(z); 
    r.values[0][1] = sin(x)*sin(y)*cos(z) + cos(x)*sin(z); 
    r.values[0][2] = (-cos(x))*sin(y)*cos(z) + sin(x)*sin(z); 

    r.values[1][0] = (-cos(y))*sin(z); 
    r.values[1][1] = (-sin(x))*sin(y)*sin(z) + cos(x)*cos(z); 
    r.values[1][2] = cos(x)*sin(y)*sin(z) + sin(x)*cos(z); 

    r.values[2][0] = sin(y); 
    r.values[2][1] = (-sin(x))*cos(y); 
    r.values[2][2] = cos(x)*cos(y); 

    return r;
}

matrix4_t get_perspective_proj_matrix(float fov, float aspect_ratio, float z_near, float z_far) {
    float f = (1/tan(fov/2));
    matrix4_t proj = {0}; 

    proj.values[0][0] = aspect_ratio * f; 
    proj.values[1][1] = f; 
    proj.values[2][2] = z_far / (z_far - z_near); 
    proj.values[2][3] = (-(z_far * z_near)) / (z_far - z_near); 
    proj.values[3][2] = 1; 

    return proj;
}

vec2_t get_perspective_projected_point(vec3_t point, matrix4_t proj_m) {
    vec4_t v = get_homogeneous_from_vec3(point); 
    v = mul_matrix4_vec4(proj_m, v); 

    // perspective divide 
    if (v.w != 0) {
        v.x /= v.w; 
        v.y /= v.w; 
        v.z /= v.w; 
    }
    vec2_t r = {.x=v.x, .y=v.y}; 

    // NDC(-1, 1) to screen space
    r.x *= window_width / 2.0; 
    r.y *= window_height / 2.0; 
    // account for inverted screen space. mesh y values +y is bottom but +y in screen space is down;
    r.y *= -1; 
    /* 
    screen origin (0, 0) is at top left while our 3D cartesian space origin (0,0,0) is middle of the screen, 
    translate all points to account for this offset between world and screen space
    */
    r.x += window_width / 2.0; 
    r.y += window_height / 2.0; 

    return r; 
}

matrix4_t get_transformation_matrix(vec3_t scale, vec3_t rotation, vec3_t translation) {
    matrix4_t s_matrix = get_scale_matrix(scale); 
    matrix4_t r_matrix = get_rotation_matrix(rotation); 
    matrix4_t t_matrix = get_translation_matrix(translation); 

    // compose scale, rotation, translation into a single transformation matrix, matrix multiply in exact order: Translate x Rotate x Scale 
    matrix4_t transformation_matrix = mul_matrix4_matrix4(r_matrix, s_matrix); 
    transformation_matrix = mul_matrix4_matrix4(t_matrix, transformation_matrix); 
    return transformation_matrix; 
}

