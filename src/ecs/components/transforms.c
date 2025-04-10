#include "transforms.h"

TransformComponent transform_components[MAX_ENTITIES];
bool has_transform[MAX_ENTITIES] = {false};             // deprecated

void add_transform(Entity e, vec3 pos, vec3 rot, vec3 scale) {
    if (e >= MAX_ENTITIES) {
        printf("ERROR: Exceeds MAX_ENTITIES!");
        return;
    }

    if (HAS_COMPONENT(e, COMPONENT_TRANSFORM)) {
        printf("WARNING: Entity: %d already has a Transform!", e);
        return;
    }

    init_position(e, pos);
    glm_vec3_copy(rot, transform_components[e].rot);
    glm_vec3_copy(scale, transform_components[e].scale);

    glm_mat4_identity(transform_components[e].mat);
    transform_components[e].parent = NO_PARENT;

    add_comp(e, COMPONENT_TRANSFORM);
}

void update_transform_matrix() {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!HAS_COMPONENT(i, COMPONENT_TRANSFORM)) {
            continue;
        }
        
        Entity parent = transform_components[i].parent;
        mat4 position, rotation, scaling;

        glm_mat4_identity(position);
        glm_mat4_identity(rotation);
        glm_mat4_identity(scaling);

        glm_mat4_identity(transform_components[i].mat);
        glm_translate(position, position_components[i].pos);
        glm_euler_xyz(transform_components[i].rot, rotation);
        glm_scale_make(scaling, transform_components[i].scale);

/*         glm_mat4_mul(transform_components[i].mat, scaling, transform_components[i].mat);
        glm_mat4_mul(transform_components[i].mat, rotation, transform_components[i].mat);
        glm_mat4_mul(transform_components[i].mat, position, transform_components[i].mat); */

        glm_mat4_mul(scaling, rotation, transform_components[i].mat);
        glm_mat4_mul(transform_components[i].mat, position, transform_components[i].mat);

        if (parent < NO_PARENT && HAS_COMPONENT(parent, COMPONENT_TRANSFORM)) {
            glm_mat4_mul(transform_components[parent].mat, transform_components[i].mat, transform_components[i].mat);
        }
    }
}

static bool is_ancestor(Entity ancestor, Entity child) {
    while (child < MAX_ENTITIES && HAS_COMPONENT(child, COMPONENT_TRANSFORM)) {
        if (transform_components[child].parent == ancestor) {
            return true;
        }
        child = transform_components[child].parent;
    }

    return false;
}

void add_parent(Entity e, Entity parent) {
    if (e >= MAX_ENTITIES || parent >= MAX_ENTITIES) {
        printf("WARNING: Entity ID out of range! (e: %d, parent: %d)\n", e, parent);
        return;
    }

    if (!HAS_COMPONENT(e, COMPONENT_TRANSFORM) || !HAS_COMPONENT(parent, COMPONENT_TRANSFORM)) {
        printf("WARNING: Entity %d or %d does not have a transform component!\n", e, parent);
        return;
    }

    if (transform_components[e].parent != NO_PARENT) {
        printf("WARNING: Entity %d already has a parent (%d)!\n", e, transform_components[e].parent);
        return;
    }

    if (is_ancestor(e, parent)) {
        printf("ERROR: Circular hierarchy detected! Cannot set entity %d as the parent of entity %d.\n", parent, e);
        return;
    }

    transform_components[e].parent = parent;
}

void remove_parent(Entity e) {
    if (e >= MAX_ENTITIES || transform_components[e].parent >= NO_PARENT) {
        printf("ERROR: Invalid entity (%d) in remove_parent().\n", e);
        return;
    }
    
    transform_components[e].parent = NO_PARENT;
}