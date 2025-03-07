#ifndef ASSET_LOADER_H
#define ASSET_LOADER_H

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <cglm/cglm.h>

#include "mesh.h"

typedef struct {
    Mesh *meshes;
    unsigned int mesh_count;
} Model;

Model load_model(const char *file_path);
void delete_model(Model *model);
void draw_model(Model *model);

#endif