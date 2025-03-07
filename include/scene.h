#ifndef SCENE_H
#define SCENE_H

#include "game_object.h"

typedef struct {
    GameObject **objects;
    unsigned int object_count;
} Scene;

Scene create_scene();
void add_game_object(Scene *scene, GameObject *object);
void delete_scene(Scene *scene);
void render_scene(Scene *scene);

#endif