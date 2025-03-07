#include "game_object.h"
#include "transform.h"

GameObject create_game_object(char *name, vec3 position, vec3 rotation, vec3 scale, Model *model, Material *material) {
    GameObject object;
    object.id = 1;
    object.name = strdup(name);
    object.model = model;
    object.material = material;
    object.transform = create_transform(position, rotation, scale);

    object.parent = NULL;
    object.children = NULL;
    object.child_count = 0;

    return object;
}

void update_game_object(GameObject *object) {
    if (!object) return;

    update_transform_matrix(&object->transform);

    if (object->parent) {
        glm_mat4_mul(object->parent->transform.matrix, object->transform.matrix, object->transform.matrix);
    }

    for (int i = 0; i < object->child_count; i++) {
        update_game_object(object->children[i]);
    }
}

void draw_game_object(GameObject *object) {
    if (!object || !object->model) return;

    glUseProgram(object->material->shader->program_id);
    set_material_uniforms(object->material);
    
    update_game_object(object);

    GLuint modelLoc = glGetUniformLocation(object->material->shader->program_id, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float *)object->transform.matrix);

    if (object->material->use_texture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE0, object->material->texture_id);
    }

    // Coloring
    GLuint inputVec3Location = glGetUniformLocation(object->material->shader->program_id, "input_color");
    glUniform3f(inputVec3Location, 0.0f, 1.0f, 0.0f);

    draw_model(object->model);

    for (int i = 0; i < object->child_count; i++) {
        draw_game_object(object->children[i]);
    }
}

void add_child(GameObject *object, GameObject *child) {
    if (!object || !child) return;

    GameObject **temp = realloc(object->children, (object->child_count + 1) * sizeof(GameObject *));
    if (temp == NULL) {
        fprintf(stderr, "Memory Allocation Error: Could not add child to %s\n", object->name);
        return;
    }

    object->children = temp;
    object->children[object->child_count] = child;
    object->child_count++;
    child->parent = object;
}

void destroy_game_object(GameObject *object) {
    if (!object) return;

    free(object->name);
    free(object->children);
    object->children = NULL;
    object->child_count = 0;
}

void move(GameObject *object, float *go) {
    glm_vec3_add(object->transform.position, go, object->transform.position);
}