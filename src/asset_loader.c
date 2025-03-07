#include <asset_loader.h>

static Mesh process_mesh(struct aiMesh *ai_mesh);

Model load_model(const char *file_path) {
    const struct aiScene *scene = aiImportFile(file_path, 
        aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_GenUVCoords);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        printf("Assimp error: %s\n", aiGetErrorString());
        exit(1);
    }

    Model model;
    model.mesh_count = scene->mNumMeshes;
    model.meshes = (Mesh *)malloc(sizeof(Mesh) * model.mesh_count);

    for (int i = 0; i < scene->mNumMeshes; i++) {
        struct aiMesh *ai_mesh = scene->mMeshes[i];
        model.meshes[i] = process_mesh(ai_mesh);
    }

    aiReleaseImport(scene);
    return model;
}

void delete_model(Model *model) {
    for (int i = 0; i < model->mesh_count; i++) {
        delete_mesh(&model->meshes[i]);
    }
    free(model->meshes);
}

void draw_model(Model *model) {
    for (int i = 0; i < model->mesh_count; i++) {
        draw_mesh(model->meshes[i]);
    }
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