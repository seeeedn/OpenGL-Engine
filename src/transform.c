#include "transform.h"

Transform create_transform(vec3 position, vec3 rotation, vec3 scale) {
    Transform t;
    glm_vec3_copy(position, t.position);
    glm_vec3_copy(rotation, t.rotation);
    glm_vec3_copy(scale, t.scale);

    glm_mat4_identity(t.matrix);
    return t;
}

void update_transform_matrix(Transform *transform) {
    mat4 translation, rotation, scaling;

    glm_mat4_identity(translation);
    glm_mat4_identity(rotation);
    glm_mat4_identity(scaling);
    glm_mat4_identity(transform->matrix);

    glm_translate(translation, transform->position);
    glm_euler_xyz(transform->rotation, rotation);
    glm_scale_make(scaling, transform->scale);

    glm_mat4_identity(transform->matrix);
    glm_mat4_mul(translation, rotation, transform->matrix);
    glm_mat4_mul(transform->matrix, scaling, transform->matrix);
}

void delete_transform(Transform *transform) {
    
    //TODO: maybe extend something here in the future

    free(transform);
}