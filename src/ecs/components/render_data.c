#include "render_data.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

RenderComponent render_components[MAX_ENTITIES];
bool has_render_comp[MAX_ENTITIES] = {false};

static Mesh create_mesh(float *vertices, size_t vertex_size, GLuint *indices, size_t index_size, size_t stride) {
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

    // TODO: add a case for stride_2 = 11;  see line 56-60

    glBindVertexArray(0);

    return mesh;
}

static Mesh process_mesh(struct aiMesh *ai_mesh) {
    int vertex_count = ai_mesh->mNumVertices;
    int index_count = ai_mesh->mNumFaces * 3;

    size_t stride = 3;

    if (ai_mesh->mNormals != NULL) stride += 3;
    if (ai_mesh->mTextureCoords[0] != NULL) stride += 2;
    if (ai_mesh->mTangents != NULL && ai_mesh->mBitangents != NULL) stride += 3;

    float *vertices = (float *)malloc(sizeof(float) * vertex_count * stride);
    unsigned int *indices = (unsigned int *)malloc(sizeof(unsigned int) * index_count);

    for (int i = 0; i < vertex_count; i++) {
        vertices[i * stride + 0] = ai_mesh->mVertices[i].x;
        vertices[i * stride + 1] = ai_mesh->mVertices[i].y;
        vertices[i * stride + 2] = ai_mesh->mVertices[i].z;

        vertices[i * stride + 3] = ai_mesh->mNormals[i].x;
        vertices[i * stride + 4] = ai_mesh->mNormals[i].y;
        vertices[i * stride + 5] = ai_mesh->mNormals[i].z;

        if (ai_mesh->mTextureCoords[0]) {
            vertices[i * stride + 6] = ai_mesh->mTextureCoords[0][i].x;
            vertices[i * stride + 7] = ai_mesh->mTextureCoords[0][i].y;
        } else {
            vertices[i * stride + 6] = 0.0f;
            vertices[i * stride + 7] = 0.0f;
        }
    }

    for (int i = 0; i < ai_mesh->mNumFaces; i++) {
        indices[i * 3 + 0] = ai_mesh->mFaces[i].mIndices[0];
        indices[i * 3 + 1] = ai_mesh->mFaces[i].mIndices[1];
        indices[i * 3 + 2] = ai_mesh->mFaces[i].mIndices[2];
    }
    
    Mesh mesh = create_mesh(vertices, vertex_count * stride * sizeof(float), indices, index_count * sizeof(unsigned int), stride);

    free(vertices);
    free(indices);

    return mesh;
}

void load_model(Entity e, const char *path) {
    if (e >= MAX_ENTITIES || has_render_comp[e]) {
        printf("ERROR: Invalid Entity %d in load_model()!\n", e);
        return;
    }

    const struct aiScene *scene = aiImportFile(path, 
        aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_GenUVCoords);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        printf("Assimp error: %s\n", aiGetErrorString());
        exit(1);
    }

    render_components[e].mesh_count = scene->mNumMeshes;
    render_components[e].meshes = (Mesh *)malloc(sizeof(Mesh) * render_components[e].mesh_count);

    for (int i = 0; i < scene->mNumMeshes; i++) {
        struct aiMesh *ai_mesh = scene->mMeshes[i];
        render_components[e].meshes[i] = process_mesh(ai_mesh);
    }

    aiReleaseImport(scene);

    has_render_comp[e] = true;
}

void delete_render_comp(Entity e) {
    if (e >= MAX_ENTITIES || !has_render_comp[e]) {
        printf("ERROR: Invalid Entity %d in delete_render_comp()!\n", e);
        return;
    }

    for (int i = 0; i < render_components[e].mesh_count; i++) {
        glDeleteVertexArrays(1, &render_components[e].meshes[i].VAO);
        glDeleteBuffers(1, &render_components[e].meshes[i].VBO);
        glDeleteBuffers(1, &render_components[e].meshes[i].EBO);

        // Set those to 0 just for safety
        render_components[e].meshes[i].index_count = 0;
        render_components[e].meshes[i].vertex_count = 0;
        render_components[e].meshes[i].texture_id = 0;
    }

    free(render_components[e].meshes);
    render_components[e].mesh_count = 0;

    delete_comp(e, COMPONENT_RENDER_DATA);
}