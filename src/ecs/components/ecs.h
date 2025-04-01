#ifndef ECS_H
#define ECS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_ENTITIES 10000
#define INVALID_ENTITY UINT32_MAX

#define COMPONENT_CAMERA        (1 << 0)
#define COMPONENT_MATERIAL      (1 << 1)
#define COMPONENT_POSITION      (1 << 2)
#define COMPONENT_RENDER_DATA   (1 << 3)
#define COMPONENT_SHADER        (1 << 4)
#define COMPONENT_TRANSFORM     (1 << 5)

typedef uint32_t Entity;
typedef uint32_t u32;

extern u32 component_mask[MAX_ENTITIES];

#define HAS_COMPONENT(entity, comp) ((component_mask[entity] & (comp)) != 0)

Entity create_entity();
void destroy_entity(Entity e);
void add_comp(Entity e, u32 comp);
void delete_comp(Enitiy e, u32 comp);

#endif