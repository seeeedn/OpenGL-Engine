#include "camera.h"

extern CameraComponent camera_components[MAX_ENTITIES];
extern bool has_camera[MAX_ENTITIES] = {false};

void init_camera(Entity e, vec3 position, vec3 up, float yaw, float pitch) {
    if (e >= MAX_ENTITIES || has_camera[e]) {
        printf("ERROR: Entity index %d exceeds MAX_ENTITIES (%d) or Entity has a Camera already!\n", e, MAX_ENTITIES);
        return;
    }

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    
    glm_vec3_copy(position, camera_components[e].position);
    glm_vec3_copy(up, camera_components[e].up);
    camera_components[e].yaw = yaw;
    camera_components[e].pitch = pitch;

    has_camera[e] = true;
    update_camera_vectors(e);
}

void update_camera_vectors(Entity e) {
    if (e >= MAX_ENTITIES || !has_camera[e]) {
        printf("ERROR: Invalid entity (%d) in update_camera_vectors().\n", e);
        return;
    }

    if (camera_components[e].pitch > 89.0f) camera_components[e].pitch = 89.0f;
    if (camera_components[e].pitch < -89.0f) camera_components[e].pitch = -89.0f;

    vec3 direction;
    direction[0] = cos(glm_rad(camera_components[e].yaw)) * cos(glm_rad(camera_components[e].pitch));
    direction[1] = sin(glm_rad(camera_components[e].pitch));
    direction[2] = sin(glm_rad(camera_components[e].yaw)) * cos(glm_rad(camera_components[e].pitch));

    glm_normalize_to(direction, camera_components[e].front);
}

void get_camera_view_matrix(Entity e, mat4 view) {
    if (e >= MAX_ENTITIES || !has_camera[e]) {
        printf("ERROR: Invalid entity (%d) in get_camera_view_matrix().\n", e);
        return;
    }

    vec3 target;
    glm_vec3_add(camera_components[e].position, camera_components[e].front, target);
    glm_lookat(camera_components[e].position, target, camera_components[e].up, view);
}

void remove_camera(Entity e) {
    if (e >= MAX_ENTITIES || !has_camera[e]) {
        printf("ERROR: Invalid entity (%d) in delete_camera().\n", e);
        return;
    }
    
    has_camera[e] = false;
}