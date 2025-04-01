#ifndef SHADER_H
#define SHADER_H

#include "ecs.h"
#include <glad/glad.h>

typedef struct {
    GLuint program_id, vertex_shader, fragment_shader;
} ShaderComponent;

extern ShaderComponent shader_components[MAX_ENTITIES];
extern bool has_shader[MAX_ENTITIES];

void init_shader(Entity e, const char* vertex_shader_source, const char* fragment_shader_source);
void delete_shader(Entity e);

#endif