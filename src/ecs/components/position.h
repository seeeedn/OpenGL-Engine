#ifndef POSITION_H
#define POSITION_H

#include "ecs.h"
#include <cglm/cglm.h>

typedef struct {
    vec3 pos;
} PositionComponent;

extern PositionComponent position_components[MAX_ENTITIES];
extern bool has_position[MAX_ENTITIES];

void init_position(Entity e, vec3 pos);
void move_to(Entity e, vec3 new_pos);
void delete_position(Entity e);

#endif