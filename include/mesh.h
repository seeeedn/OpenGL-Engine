#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

typedef struct {
    GLuint VAO, VBO, EBO;
    unsigned int vertex_count, index_count;
} Mesh;

Mesh create_mesh(float *verticies, size_t vertex_size, GLuint *indices, size_t index_size, size_t stride);
void draw_mesh(Mesh mesh);
void delete_mesh(Mesh *mesh);

#endif