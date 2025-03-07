#include "ecs.h"

static Entity next_entity = 0;
static bool entity_alive[MAX_ENTITIES];
static Entity free_entities[MAX_ENTITIES];
static int free_entity_count = 0;

Entity create_entity() {
    if (free_entity_count > 0) {
        Entity e = free_entities[free_entity_count--];
        entity_alive[e] = true;
        return e;
    }
    
    if (next_entity < MAX_ENTITIES) {
        entity_alive[next_entity] = true;
        return next_entity++;
    }

    printf("ERROR: Maximum entity count reached!\n");
    return (Entity)-1;
}

void delete_entity(Entity e) {
    if (e >= MAX_ENTITIES || !entity_alive[e]) {
        printf("WARNING: Tried to destroy a non-existent entity %u\n", e);
        return;
    }

    entity_alive[e] = false;

    free_entities[free_entity_count++] = e;
}