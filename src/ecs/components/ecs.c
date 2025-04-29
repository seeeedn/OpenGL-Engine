#include "ecs.h"

static Entity next_entity = 0;
static bool entity_alive[MAX_ENTITIES];


static Entity free_entities[MAX_ENTITIES];
static int free_entity_count = MAX_ENTITIES;
u32 component_mask[MAX_ENTITIES] = {0};

// needs to be refractored
Entity create_entity() {
    if (free_entity_count > 0) {
        Entity e = free_entities[--free_entity_count];
        entity_alive[e] = true;
        ADD_COMPONENT(e, USED_ENTITY);
        return e;
    }
    
    if (next_entity < MAX_ENTITIES) {
        entity_alive[next_entity] = true;
        return next_entity++;
    }

    printf("ERROR: Maximum entity count reached!\n");

    return INVALID_ENTITY;
}

void delete_entity(Entity e) {
    if (e >= MAX_ENTITIES || !entity_alive[e]) {
        printf("WARNING: Tried to destroy a non-existent entity %u!\n", e);
        return;
    }

    component_mask[e] = 0;
    entity_alive[e] = false;
    free_entities[free_entity_count++] = e;
    DEL_COMPONENT(e, USED_ENTITY);
}

void add_comp(Entity e, u32 comp) {
    if (e >= MAX_ENTITIES) {
        printf("WARNING: Tried to access a non-existent entity %u!\n", e);
        return;
    }

    component_mask[e] |= comp;
}

void delete_comp(Entity e, u32 comp) {
    if (e >= MAX_ENTITIES) {
        printf("WARNING: Tried to access a non-existent entity %u!\n", e);
        return;
    }

    component_mask[e] &= ~comp;
}