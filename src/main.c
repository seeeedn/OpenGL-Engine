#include <stdio.h>
#include <glad/glad.h>
#include <cglm/cglm.h>

#include "ecs/components/shader.h"
#include "ecs/components/camera.h"
#include "ecs/components/render_data.h"
#include "ecs/components/render_system.h"
#include "ecs/components/transforms.h"
#include "ecs/components/position.h"
#include "ecs/components/ecs.h"
#include "sdl_window.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

vec3 camera_pos = {0.0f, 0.0f, 3.0f};       // Camera start position
vec3 camera_front = {0.0f, 0.0f, -1.0f};    // Direction camera is facing
vec3 camera_up = {0.0f, 1.0f, 0.0f};        // Up vector

float yaw = -90.0f;  // Yaw (horizontal rotation)
float pitch = 0.0f;  // Pitch (vertical rotation)
float speed = 3.0f;
float sensitivity = 0.05f;

mat4 projection, view;

char title[64];

int main(int argc, char *argv[]) {
    Window window = init_window("OpenGL-Engine", SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    const char *vertex_shader_source = "../shaders/shader.vert";
    const char *fragment_shader_source = "../shaders/shader.frag";

    init_shader(1, vertex_shader_source, fragment_shader_source);

    load_model(1, "../assets/models/pdr.glb");

    vec3 position = {0.0f, 0.0f, -50.0f};
    vec3 rotation = {0.0f, 0.0f, 0.0f};
    vec3 scale = {1.0f, 1.0f, 1.0f};

    vec3 new_pos = {0.0f, 50.0f, -50.0f};

    add_transform(1, position, rotation, scale);

    int target_fps = 144;
    int frame_delay = 1000 / target_fps;
    Uint64 last = 0;
    float delta_time = 0;

    init_camera(0, camera_pos, camera_up, yaw, pitch);
    //move_to(1, new_pos);

/*     Entity e = create_entity();
    Entity e2 = create_entity();
    printf("e1: %d\ne2: %d\n", e, e2);

    delete_entity(e);
    Entity e3 = create_entity();
    printf("new e: %d", e3); */

    bool running = true;
    while (running) {
        delta_time = get_delta_time(&last);

        window_poll_events(&window, &running);
        //handle_movement_input(&camera, get_button_pressed(), delta_time);
        change_render_mode();

// ------------------------------------------------------------------------------------------------ //
//                                    RENDERING  BELOW  HERE                                        //
// ------------------------------------------------------------------------------------------------ //
        render();

        window_swap_buffers(&window);
        //limit_frame_rate(delta_time, target_fps);
    }

    destroy_window(&window);
}