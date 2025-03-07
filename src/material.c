#include "material.h"

void set_material_uniforms(Material *material) {
    glUseProgram(material->shader->program_id);

    glUniform3fv(glGetUniformLocation(material->shader->program_id, "material.ambient"), 1, &material->ambient[0]);
    glUniform3fv(glGetUniformLocation(material->shader->program_id, "material.diffuse"), 1, &material->diffuse[0]);
    glUniform3fv(glGetUniformLocation(material->shader->program_id, "material.specular"), 1, &material->specular[0]);
    glUniform1f(glGetUniformLocation(material->shader->program_id, "material.shininess"), material->shininess);

    if (material->use_texture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material->texture_id);
        glUniform1i(glGetUniformLocation(material->shader->program_id, "material.texture"), 0);
    }
}