#include "render_system.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

void render() {
    mat4 projection, view;

    glm_perspective(glm_rad(45.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f, projection);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    update_transform_matrix();
    get_camera_view_matrix(0, view);

    GLuint last_shader_prog = 0;

    for (int i = 0; i < MAX_ENTITIES; i++) {
        if (!has_transform[i]) {
            continue;
        }
        
        GLuint current_shader_prog = shader_components[i].program_id;

        if (current_shader_prog != last_shader_prog) {
            glUseProgram(current_shader_prog);
            last_shader_prog = current_shader_prog;
        }

        GLuint viewLoc = glGetUniformLocation(current_shader_prog, "view");
        GLuint projLoc = glGetUniformLocation(current_shader_prog, "projection");
        GLuint modelLoc = glGetUniformLocation(current_shader_prog, "model");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float *)view);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float *)projection);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float *)transform_components[i].mat);

        // TODO: has to be refractored later on
        GLuint inputVec3Location = glGetUniformLocation(current_shader_prog, "input_color");
        glUniform3f(inputVec3Location, 0.0f, 1.0f, 0.0f);

        for (int j = 0; j < render_components[i].mesh_count; j++) {
            Mesh *mesh = &render_components[i].meshes[j];

            glBindVertexArray(mesh->VAO);
            if (mesh->index_count > 0) {
                // render with EBO
                glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);
            } else {
                // render with VAO
                glDrawArrays(GL_TRIANGLES, 0, mesh->vertex_count);
            }
            glBindVertexArray(0);
        }
    }
}