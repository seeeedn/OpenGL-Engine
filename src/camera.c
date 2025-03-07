#include "camera.h"

void init_camera(Camera *camera, vec3 position, vec3 up, float yaw, float pitch, float speed, float sensitivity) {
    glm_vec3_copy(position, camera->position);
    glm_vec3_copy(up, camera->up);
    camera->yaw = yaw;
    camera->pitch = pitch;
    camera->speed = speed;
    camera->sensitivity = sensitivity;
    update_camera_vectors(camera);
}

void update_camera_vectors(Camera *camera) {
    vec3 direction;
    direction[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    direction[1] = sin(glm_rad(camera->pitch));
    direction[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));

    glm_normalize_to(direction, camera->front);
}

void handle_movement_input(Camera *camera, const Uint8 *keystate, float delta_time) {
    float velocity = camera->speed * delta_time;
    vec3 movement = {0.0f, 0.0f, 0.0f};
    vec3 camera_right;
    glm_cross(camera->front, camera->up, camera_right);
    glm_normalize(camera_right);

    if (keystate[SDL_SCANCODE_W]) glm_vec3_add(movement, camera->front, movement);
    if (keystate[SDL_SCANCODE_S]) glm_vec3_sub(movement, camera->front, movement);
    if (keystate[SDL_SCANCODE_A]) glm_vec3_sub(movement, camera_right, movement);
    if (keystate[SDL_SCANCODE_D]) glm_vec3_add(movement, camera_right, movement);

    if (!glm_vec3_eqv_eps(movement, (vec3){0.0f, 0.0f, 0.0f})) {
        glm_normalize(movement);
        glm_vec3_scale(movement, velocity, movement);
        glm_vec3_add(camera->position, movement, camera->position);
    }
}

void handle_mouse_input(Camera *camera, float x_offset, float y_offset) {
    x_offset *= camera->sensitivity;
    y_offset *= camera->sensitivity * -1;

    camera->yaw += x_offset;
    camera->pitch += y_offset;

    if (camera->pitch > 89.0f) camera->pitch = 89.0f;
    if (camera->pitch < -89.0f) camera->pitch = -89.0f;

    update_camera_vectors(camera);
}

void get_view_matrix(Camera *camera, mat4 view) {
    vec3 target;
    glm_vec3_add(camera->position, camera->front, target);
    glm_lookat(camera->position, target, camera->up, view);
}