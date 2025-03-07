#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>
#include <SDL2/SDL.h>

typedef struct {
    vec3 position, front, up;
    float speed, yaw, pitch, sensitivity;
    mat4 view;
} Camera;

void init_camera(Camera *camera, vec3 position, vec3 up, float yaw, float pitch, float speed, float sensitivity);
void update_camera_vectors(Camera *camera);
void handle_movement_input(Camera *camera, const Uint8 *keystate, float delta_time);
void handle_mouse_input(Camera *camera, float x_offset, float y_offset);
void get_view_matrix(Camera *camera, mat4 view);

#endif