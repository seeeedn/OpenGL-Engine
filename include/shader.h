#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

typedef struct {
    GLuint program_id, vertex_shader, fragment_shader;
} ShaderProgram;

char* read_file(const char *path);
GLuint compile_shader(const char *shader_source, GLenum shader_type);
GLuint link_shader_program(GLuint vertex_shader, GLuint fragment_shader);
ShaderProgram create_shader_program(const char *vertex_shader_source, const char *fragment_shader_source);
void use_shader(ShaderProgram *shader_program);

#endif