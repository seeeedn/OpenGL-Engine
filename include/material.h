#ifndef MATERIAL_H
#define MATERIAL_H

#include "shader.h"
#include "cglm/cglm.h"

typedef struct {
    ShaderProgram *shader;
    GLuint texture_id;
    int use_texture;
    vec3 ambient, diffuse, specular;
    GLfloat shininess;
} Material;

void set_material_uniforms(Material *material);

#endif