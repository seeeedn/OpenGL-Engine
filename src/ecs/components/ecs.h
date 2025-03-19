#ifndef ECS_H
#define ECS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_ENTITIES 10000
#define INVALID_ENTITY UINT32_MAX

typedef uint32_t Entity;

Entity create_entity();
void destroy_entity(Entity e);

#endif