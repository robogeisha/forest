#include "camera.h"
#include <cglm/cam.h>
#include <cglm/vec3.h>

void camera_init(Camera *camera, vec3 position, vec3 up, vec3 front, float fov, float aspect_ratio, float near_plane, float far_plane) {
    glm_vec3_copy(position, camera->position);
    glm_vec3_copy(up, camera->up);
    glm_vec3_copy(front, camera->front);
    camera->fov = fov;
    camera->aspect_ratio = aspect_ratio;
    camera->near_plane = near_plane;
    camera->far_plane = far_plane;
}

void camera_get_view_matrix(const Camera *camera, mat4 view) {
    glm_lookat((vec3){camera->position[0], camera->position[1], camera->position[2]},
               (vec3){camera->front[0], camera->front[1], camera->front[2]},
               (vec3){camera->up[0], camera->up[1], camera->up[2]}, view);
}

void camera_get_projection_matrix(const Camera *camera, mat4 projection) {
    glm_perspective(camera->fov, camera->aspect_ratio, camera->near_plane, camera->far_plane, projection);
}
