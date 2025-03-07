#ifndef CAMERA_H
#define CAMERA_H

#include "ecs.h"
#include <cglm/cglm.h>

typedef struct {
    vec3 position, front, up;
    float yaw, pitch;
    mat4 view;
} CameraComponent;

extern CameraComponent camera_components[MAX_ENTITIES];
extern bool has_camera[MAX_ENTITIES];

void init_camera(Entity e, vec3 position, vec3 up, float yaw, float pitch);
void update_camera_vectors(Entity e);
void get_camera_view_matrix(Entity e, mat4 view);
void remove_camera(Entity e);

#endif