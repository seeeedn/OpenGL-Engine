#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <cglm/cglm.h>

typedef struct {
    vec3 position, rotation, scale;
    mat4 matrix;
} Transform;

Transform create_transform(vec3 position, vec3 rotation, vec3 scale);
void update_transform_matrix(Transform *transform);
void delete_transform(Transform *transform);

#endif