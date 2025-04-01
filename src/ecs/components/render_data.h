#ifndef RENDER_DATA_H
#define RENDER_DATA_H

#include "ecs.h"
#include <glad/glad.h>

typedef struct {
    GLuint VAO, VBO, EBO, texture_id;
    unsigned int vertex_count, index_count;
} Mesh;

typedef struct {
    Mesh* meshes;
    size_t mesh_count;
} RenderComponent;

extern RenderComponent render_components[MAX_ENTITIES];
extern bool has_render_comp[MAX_ENTITIES];

void load_model(Entity e, const char *path);
void delete_render_comp(Entity e);

#endif