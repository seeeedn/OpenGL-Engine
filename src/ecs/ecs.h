#ifndef ECS_H
#define ECS_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_ENTITIES 10000

typedef uint32_t Entity;

Entity create_entity();
void destroy_entity(Entity e);

#endif