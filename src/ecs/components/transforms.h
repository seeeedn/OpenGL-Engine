#ifndef TRANSFORMS_H
#define TRANSFORMS_H

#include "ecs.h"
#include "position.h"
#include <cglm/cglm.h>

#define NO_PARENT MAX_ENTITIES

typedef struct {
    vec3 pos, rot, scale;
    mat4 mat;
    Entity parent;
} TransformComponent;

extern TransformComponent transform_components[MAX_ENTITIES];
extern bool has_transform[MAX_ENTITIES];        // deprecated

void add_transform(Entity e, vec3 pos, vec3 rot, vec3 scale);
void update_transform_matrix();
void add_parent(Entity e, Entity parent);
void remove_parent(Entity e);

#endif