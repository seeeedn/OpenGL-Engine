#include "position.h"

PositionComponent position_components[MAX_ENTITIES];
bool has_position[MAX_ENTITIES] = {false};

void init_position(Entity e, vec3 pos) {
    if (e >= MAX_ENTITIES || has_position[e]) {
        printf("ERROR: Invalid Entity %d in init_position()!", e);
        return;
    }
 
    glm_vec3_copy(pos, position_components[e].pos);

    has_position[e] = true;
}

void move_to(Entity e, vec3 new_pos) {
    if (e >= MAX_ENTITIES || !has_position[e]) {
        printf("ERROR: Invalid Entity %d in move()!", e);
        return;
    }

    glm_vec3_copy(new_pos, position_components[e].pos);
}

void delete_position(Entity e) {
    if (e >= MAX_ENTITIES || !has_position[e]) {
        printf("ERROR: Invalid Entity %d in delete_position()!", e);
        return;
    }

    vec3 del_vec = {0.0f, 0.0f, 0.0f};
    glm_vec3_copy(del_vec, position_components[e].pos);

    has_position[e] = false;
}