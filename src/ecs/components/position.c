#include "position.h"

PositionComponent position_components[MAX_ENTITIES];
bool has_position[MAX_ENTITIES] = {false};

void init_positon(Entity e, int x, int y, int z) {
    if (e >= MAX_ENTITIES || has_position[e]) {
        printf("ERROR: Invalid Entity %d in init_position()!", e);
        return;
    }



    position_components[e].x = x;
    position_components[e].y = y;
    position_components[e].z = z;

    has_position[e] = true;
}

void move() {

}

void delete_position(Entity e) {
    if (e >= MAX_ENTITIES || !has_position[e]) {
        printf("ERROR: Invalid Entity %d in delete_position()!", e);
        return;
    }

    position_components[e].x = 0;
    position_components[e].y = 0;
    position_components[e].z = 0;

    has_position[e] = false;
}