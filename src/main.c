#include <stdio.h>
#include <glad/glad.h>
#include <cglm/cglm.h>

#include "shader.h"
#include "mesh.h"
#include "camera.h"
#include "asset_loader.h"
#include "sdl_window.h"
#include "game_object.h"
#include "scene.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

typedef int Entity;

vec3 camera_pos = {0.0f, 0.0f, 3.0f};  // Camera start position
vec3 camera_front = {0.0f, 0.0f, -1.0f};  // Direction camera is facing
vec3 camera_up = {0.0f, 1.0f, 0.0f};  // Up vector

float yaw = -90.0f;  // Yaw (horizontal rotation)
float pitch = 0.0f;  // Pitch (vertical rotation)
float speed = 3.0f;
float sensitivity = 0.05f;

mat4 projection, view;
Camera camera;
char title[64];

int main(int argc, char *argv[]) {
    Window window = init_window("OpenGL", SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    const char *vertex_shader_source = read_file("C:/Users/jonas/OneDrive/Desktop/OpenGL_test/shaders/shader.vert");
    const char *fragment_shader_source = read_file("C:/Users/jonas/OneDrive/Desktop/OpenGL_test/shaders/shader.frag");
    ShaderProgram shader_program = create_shader_program(vertex_shader_source, fragment_shader_source);

    Model test_model = load_model("C:/Users/jonas/OneDrive/Desktop/OpenGL_test/assets/models/pdr.glb");

    Material test_material = {
        .shader = &shader_program,
        .texture_id = 0,
        .ambient = {0.2f, 0.2f, 0.2f},
        .diffuse = {0.8f, 0.8f, 0.8f},
        .specular = {1.0f, 1.0f, 1.0f},
        .shininess = 32.0f,
        .use_texture = false
    };

    vec3 position = {0.0f, 0.0f, 0.0f};
    vec3 rotation = {0.0f, 0.0f, 0.0f};
    vec3 scale = {1.0f, 1.0f, 1.0f};
    GameObject test_object = create_game_object("TestObject_1", position, rotation, scale, &test_model, &test_material);

    vec3 position_2 = {20.0f, 5.0f, 0.0f};
    vec3 rotation_2 = {1.0f, 1.0f, 0.0f};
    GameObject test_object_2 = create_game_object("TestObject_2", position_2, rotation_2, scale, &test_model, &test_material);

    int target_fps = 144;
    int frame_delay = 1000 / target_fps;
    Uint64 last = 0;
    float delta_time = 0;

    // Transformation Matricies
    glm_perspective(glm_rad(45.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f, projection);
    GLuint viewLoc = glGetUniformLocation(shader_program.program_id, "view");
    GLuint projLoc = glGetUniformLocation(shader_program.program_id, "projection");

    init_camera(&camera, camera_pos, camera_up, yaw, pitch, speed, sensitivity);

    add_child(&test_object, &test_object_2);

    Scene test_scene = create_scene();
    add_game_object(&test_scene, &test_object);

    vec3 pos = {0.0f, 0.0f, 0.0002f};

    int running = 1;
    while (running) {
        delta_time = get_delta_time(&last);

        window_poll_events(&window, &running, &camera);
        handle_movement_input(&camera, get_button_pressed(), delta_time);
        change_render_mode();

// ------------------------------------------------------------------------------------------------ //
//                                    RENDERING  BELOW  HERE                                        //
// ------------------------------------------------------------------------------------------------ //
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        get_view_matrix(&camera, view);

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float *)view);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float *)projection);

        render_scene(&test_scene);

        move(&test_object, pos);

        window_swap_buffers(&window);
    }

    delete_model(&test_model);
    glDeleteProgram(shader_program.program_id);
    destroy_window(&window);
}