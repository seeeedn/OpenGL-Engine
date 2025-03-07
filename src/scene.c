#include "scene.h"

Scene create_scene() {
    Scene scene;
    scene.objects = NULL;
    scene.object_count = 0;

    return scene;
}

void add_game_object(Scene *scene, GameObject *object) {
    GameObject **temp = realloc(scene->objects, (scene->object_count + 1) * sizeof(GameObject *));
    if (temp == NULL) {
        fprintf(stderr, "Memory Allocation Error\n");
        return;
    }

    scene->objects = temp;
    scene->objects[scene->object_count] = object;
    scene->object_count++;
}

void delete_scene(Scene *scene) {
    free(scene->objects);
    scene->object_count = 0;
}

void render_scene(Scene *scene) {
    for (int i = 0; i < scene->object_count; i++) {
        draw_game_object(scene->objects[i]);
    }
}