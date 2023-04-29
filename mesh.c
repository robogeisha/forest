#include "mesh.h"
#include <stdlib.h>
#include "forest_environment.h"
#include "lsystem.h"

Mesh* create_mesh(GLfloat* vertices, size_t vertex_count, GLuint* indices, size_t index_count) {
    Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
    if (!mesh) {
        return NULL;
    }

    mesh->vertices = vertices;
    mesh->vertex_count = vertex_count;
    mesh->indices = indices;
    mesh->index_count = index_count;

    return mesh;
}

void create_terrain_mesh(Mesh **terrain_mesh, float terrain[TERRAIN_SIZE][TERRAIN_SIZE]) {
    // Calculate the total number of vertices and indices.
    size_t vertex_count = TERRAIN_SIZE * TERRAIN_SIZE * 3;
    size_t index_count = (TERRAIN_SIZE - 1) * (TERRAIN_SIZE - 1) * 6;

    // Allocate memory for vertices and indices.
    GLfloat *vertices = (GLfloat *)malloc(vertex_count * sizeof(GLfloat));
    GLuint *indices = (GLuint *)malloc(index_count * sizeof(GLuint));

    if (!vertices || !indices) {
        free(vertices);
        free(indices);
        *terrain_mesh = NULL;
        return;
    }

    // Fill the vertices array.
    int vertex_index = 0;
    for (int z = 0; z < TERRAIN_SIZE; z++) {
        for (int x = 0; x < TERRAIN_SIZE; x++) {
            vertices[vertex_index++] = (GLfloat)x;
            vertices[vertex_index++] = terrain[x][z];
            vertices[vertex_index++] = (GLfloat)z;
        }
    }

    // Fill the indices array.
    int index_index = 0;
    for (int z = 0; z < TERRAIN_SIZE - 1; z++) {
        for (int x = 0; x < TERRAIN_SIZE - 1; x++) {
            int current_vertex = z * TERRAIN_SIZE + x;
            indices[index_index++] = current_vertex;
            indices[index_index++] = current_vertex + TERRAIN_SIZE;
            indices[index_index++] = current_vertex + TERRAIN_SIZE + 1;

            indices[index_index++] = current_vertex;
            indices[index_index++] = current_vertex + TERRAIN_SIZE + 1;
            indices[index_index++] = current_vertex + 1;
        }
    }

    // Create the mesh using the vertices and indices.
    *terrain_mesh = create_mesh(vertices, vertex_count, indices, index_count);
}


#include "forest_environment.h"

void create_tree_mesh(Mesh **tree_mesh, TreeInstance *tree_instance) {
    // Define the basic shape of the tree as a simple cylinder (trunk) and cone (leaves).
    const int cylinder_segments = 12;
    const int cone_segments = 12;

    const float trunk_radius = 0.1f;
    const float trunk_height = 1.0f;

    const float leaves_radius = 0.5f;
    const float leaves_height = 1.5f;

    // Calculate the total number of vertices and indices.
    size_t vertex_count = (cylinder_segments + 1) * 2 + (cone_segments + 1) * 2;
    size_t index_count = cylinder_segments * 6 + cone_segments * 3;

    // Allocate memory for vertices and indices.
    GLfloat *vertices = (GLfloat *)malloc(vertex_count * sizeof(GLfloat));
    GLuint *indices = (GLuint *)malloc(index_count * sizeof(GLuint));

    if (!vertices || !indices) {
        free(vertices);
        free(indices);
        *tree_mesh = NULL;
        return;
    }

    // Fill the vertices array with the trunk and leaves.
    int vertex_index = 0;
    for (int i = 0; i <= cylinder_segments; i++) {
        float angle = 2.0f * M_PI * (float)i / (float)cylinder_segments;

        float x = trunk_radius * cos(angle);
        float z = trunk_radius * sin(angle);

        // Bottom vertex of the trunk
        vertices[vertex_index++] = x;
        vertices[vertex_index++] = 0.0f;
        vertices[vertex_index++] = z;

        // Top vertex of the trunk
        vertices[vertex_index++] = x;
        vertices[vertex_index++] = trunk_height;
        vertices[vertex_index++] = z;
    }

    // Fill the vertices array with the leaves.
    for (int i = 0; i <= cone_segments; i++) {
        float angle = 2.0f * M_PI * (float)i / (float)cone_segments;

        float x = leaves_radius * cos(angle);
        float z = leaves_radius * sin(angle);

        // Bottom vertex of the leaves
        vertices[vertex_index++] = x;
        vertices[vertex_index++] = trunk_height;
        vertices[vertex_index++] = z;

        // Top vertex of the leaves (all sharing the same position)
        vertices[vertex_index++] = 0.0f;
        vertices[vertex_index++] = trunk_height + leaves_height;
        vertices[vertex_index++] = 0.0f;
    }

    // Fill the indices array with the trunk and leaves.
    int index_index = 0;
    for (int i = 0; i < cylinder_segments; i++) {
        indices[index_index++] = i * 2;
        indices[index_index++] = (i * 2 + 2) % (cylinder_segments * 2);
        indices[index_index++] = i * 2 + 1;

        indices[index_index++] = i * 2 + 1;
        indices[index_index++] = (i * 2 + 2) % (cylinder_segments * 2);
        indices[index_index++] = (i * 2 + 3)        % (cylinder_segments * 2);
    }

    for (int i = 0; i < cone_segments; i++) {
        indices[index_index++] = cylinder_segments * 2 + i * 2;
        indices[index_index++] = cylinder_segments * 2 + (i * 2 + 2) % (cone_segments * 2);
        indices[index_index++] = cylinder_segments * 2 + i * 2 + 1;
    }

    // Create the mesh using the vertices and indices.
    *tree_mesh = create_mesh(vertices, vertex_count, indices, index_count);
}



void free_mesh(Mesh* mesh) {
    if (!mesh) {
        return;
    }

    if (mesh->vertices) {
        free(mesh->vertices);
    }

    if (mesh->indices) {
        free(mesh->indices);
    }

    free(mesh);
}

void render_mesh(Mesh* mesh) {
    if (!mesh) {
        return;
    }

    // Use the shader program, bind VAO, and render the mesh here.
    // The actual implementation will depend on your VAO setup and how
    // you handle shaders and uniforms.
}
