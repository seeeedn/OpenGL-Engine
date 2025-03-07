#include <stdio.h>
#include <stdlib.h>
#include "shader.h"

char* read_file(const char *path) {
    FILE *file = fopen(path, "r");

    if (!file) {
        printf("Error!");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char *buffer = (char*)malloc(length + 1);
    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}

GLuint compile_shader(const char *shader_source, GLenum shader_type) {
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        printf("Shader Compilation failed: %s", infoLog);
    }

    return shader;
}

GLuint link_shader_program(GLuint vertex_shader, GLuint fragment_shader) {
    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader);
    glAttachShader(program_id, fragment_shader);
    glLinkProgram(program_id);
    
    int success;
    char infoLog[1024];
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program_id, 1024, NULL, infoLog);
        printf("Shader Program linking failed: %s", infoLog);
    }

    return program_id;
}

ShaderProgram create_shader_program(const char *vertex_shader_source, const char *fragment_shader_source) {
    GLuint vertex_shader = compile_shader(vertex_shader_source, GL_VERTEX_SHADER);
    GLuint fragment_shader = compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);
    GLuint program_id = link_shader_program(vertex_shader, fragment_shader);

    return (ShaderProgram){program_id, vertex_shader, fragment_shader};
}

void use_shader(ShaderProgram *shader_program) {
    glUseProgram(shader_program->program_id);
}