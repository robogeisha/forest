#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shader.h"
#include "utils.h"




static GLuint compile_shader(const char* shader_source, GLenum shader_type) {
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        fprintf(stderr, "Shader compilation error: %s\n", info_log);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint create_shader_program(const char* vertex_shader_path, const char* fragment_shader_path) {
    char* vertex_shader_source = read_file(vertex_shader_path);
    char* fragment_shader_source = read_file(fragment_shader_path);

    if (!vertex_shader_source || !fragment_shader_source) {
        free(vertex_shader_source);
        free(fragment_shader_source);
        return 0;
    }

    GLuint vertex_shader = compile_shader(vertex_shader_source, GL_VERTEX_SHADER);
    GLuint fragment_shader = compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);

    free(vertex_shader_source);
    free(fragment_shader_source);

    if (!vertex_shader || !fragment_shader) {
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        return 0;
    }

    GLuint shader_program = glCreateProgram();


    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    GLint success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        fprintf(stderr, "Shader program linking error: %s\n", info_log);
        glDeleteProgram(shader_program);
        shader_program = 0;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}
