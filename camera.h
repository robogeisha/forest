#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>

typedef struct Camera {
    vec3 position;
    vec3 up;
    vec3 front;
    float fov;
    float aspect_ratio;
    float near_plane;
    float far_plane;
} Camera;

void camera_init(Camera *camera, vec3 position, vec3 up, vec3 front, float fov, float aspect_ratio, float near_plane, float far_plane);
void camera_get_view_matrix(const Camera *camera, mat4 view);
void camera_get_projection_matrix(const Camera *camera, mat4 projection);

#endif // CAMERA_H
