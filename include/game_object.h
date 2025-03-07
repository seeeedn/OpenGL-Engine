#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "asset_loader.h"
#include "mesh.h"
#include "transform.h"
#include "material.h"

typedef struct GameObject {
    unsigned int id;
    char *name;
    Model *model;
    Material *material;
    Transform transform;
    /* components */
    struct GameObject *parent;
    struct GameObject **children;
    unsigned int child_count;
} GameObject;

GameObject create_game_object(char *name, vec3 position, vec3 rotation, vec3 scale, Model *model, Material *material);
void destroy_game_object(GameObject *object);
//void add_component(GameObject *object, /* component */);
void update_game_object(GameObject *object);
void draw_game_object(GameObject *object);
void add_child(GameObject *object, GameObject *child);
void set_name(GameObject *object, char *new_name);

void move(GameObject *object, float *go);

#endif