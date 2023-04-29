#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

GLuint create_shader_program(const char* vertex_shader_path, const char* fragment_shader_path);

#endif // SHADER_H
