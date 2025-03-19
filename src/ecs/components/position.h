#ifndef POSITION_H
#define POSITION_H

#include "ecs.h"

typedef struct {
    int x, y, z;
} PositionComponent;

extern PositionComponent position_components[MAX_ENTITIES];
extern bool has_position[MAX_ENTITIES];

void init_position(Entity e, int x, int y, int z);
void move(Entity e);
void delete_position(Entity e);

#endif