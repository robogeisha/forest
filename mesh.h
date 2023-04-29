#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

typedef struct {
    GLfloat* vertices;
    GLuint* indices;
    size_t vertex_count;
    size_t index_count;
} Mesh;

Mesh* create_mesh(GLfloat* vertices, size_t vertex_count, GLuint* indices, size_t index_count);
void free_mesh(Mesh* mesh);
void render_mesh(Mesh* mesh);

#endif // MESH_H
