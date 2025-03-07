#include "mesh.h"

Mesh create_mesh(float *vertices, size_t vertex_size, GLuint *indices, size_t index_size, size_t stride) {
    Mesh mesh;

    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);

    glBindVertexArray(mesh.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex_size, vertices, GL_STATIC_DRAW);

    if (indices != NULL) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, indices, GL_STATIC_DRAW);
        mesh.index_count = index_size / sizeof(GLuint);
    } else {
        mesh.index_count = 0;
    }


    mesh.vertex_count = vertex_size / (stride * sizeof(float));
    size_t stride_2 = vertex_size / mesh.vertex_count;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride_2, (void*)0);
    glEnableVertexAttribArray(0);

    if (stride_2 >= 6 * sizeof(float)) {
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride_2, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    if (stride_2 >= 8 * sizeof(float)) {
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride_2, (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    glBindVertexArray(0);

    return mesh;
}

void draw_mesh(Mesh mesh) {
    glBindVertexArray(mesh.VAO);

    if (mesh.index_count > 0) {
        // render with EBO
        glDrawElements(GL_TRIANGLES, mesh.index_count, GL_UNSIGNED_INT, 0);
    } else {
        // render with VAO
        glDrawArrays(GL_TRIANGLES, 0, mesh.vertex_count);
    }

    glBindVertexArray(0);
}

void delete_mesh(Mesh *mesh) {
    glDeleteVertexArrays(1, &mesh->VAO);
    glDeleteBuffers(1, &mesh->VBO);
    glDeleteBuffers(1, &mesh->EBO);
}