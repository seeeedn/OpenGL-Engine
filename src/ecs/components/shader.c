#include "shader.h"

ShaderComponent shader_components[MAX_ENTITIES];
bool has_shader[MAX_ENTITIES] = {false};

static char* read_file(const char* path) {
    FILE *file = fopen(path, "r");

    if (!file) {
        printf("ERROR: Failed to open file: %s\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char *shader_buffer = (char*)malloc(length + 1);
    if (!shader_buffer) {
        printf("ERROR: Memory allocation failed!\n");
        fclose(file);
        return NULL;
    }

    size_t read_size = fread(shader_buffer, 1, length, file);
    if (read_size != (size_t)length) {
        printf("ERROR: Could not read the full file: %s\n", path);
        free(shader_buffer);
        fclose(file);
        return NULL;
    }

    shader_buffer[length] = '\0';
    fclose(file);

    return shader_buffer;
}

static GLuint compile_shader(const char *shader_source, GLenum shader_type) {
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        printf("Shader Compilation failed: %s\n", infoLog);
    }

    return shader;
}

static GLuint link_shader_program(GLuint vertex_shader, GLuint fragment_shader) {
    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader);
    glAttachShader(program_id, fragment_shader);
    glLinkProgram(program_id);
    
    int success;
    char infoLog[1024];
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program_id, 1024, NULL, infoLog);
        printf("Shader Program linking failed: %s\n", infoLog);
    }

    return program_id;
}

void init_shader(Entity e, const char *vertex_shader_source, const char *fragment_shader_source) {
    if (e >= MAX_ENTITIES) {
        printf("ERROR: Invalid Entity %d\n", e);
        return;
    }

    if (!vertex_shader_source || !fragment_shader_source) {
        printf("ERROR: NULL Pointer!\n");
        return;
    }

    const char *vertex_shader = read_file(vertex_shader_source);
    const char *fragment_shader = read_file(fragment_shader_source);

    if (!vertex_shader || !fragment_shader) {
        printf("ERROR: Failed to load shader source files!\n");
        free(vertex_shader);
        free(fragment_shader);
        return;
    }

    shader_components[e].vertex_shader = compile_shader(vertex_shader, GL_VERTEX_SHADER);
    shader_components[e].fragment_shader = compile_shader(fragment_shader, GL_FRAGMENT_SHADER);
    shader_components[e].program_id = link_shader_program(shader_components[e].vertex_shader, shader_components[e].fragment_shader);

    free(vertex_shader);
    free(fragment_shader);

    has_shader[e] = true;
}

void delete_shader(Entity e) {
    if (e >= MAX_ENTITIES || !has_shader[e]) {
        printf("ERROR: Invalid Entity %d\n", e);
        return;
    }

    glDeleteShader(shader_components[e].vertex_shader);
    glDeleteShader(shader_components[e].fragment_shader);
    glDeleteProgram(shader_components[e].program_id);

    shader_components[e].vertex_shader = 0;
    shader_components[e].fragment_shader = 0;
    shader_components[e].program_id = 0;

    has_shader[e] = false;
}