#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <math.h>
#include "forest_environment.h"
#include "mesh.h"
#include "camera.h"
#include "perlin.h"
#include "utils.h"
#include "shader.h"
#include "lsystem.h"
#include "input_callbacks.h"


#define SCR_WIDTH 800
#define SCR_HEIGHT 600
#define NUM_CONE_SEGMENTS 12

void process_input(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void create_tree_mesh(Mesh **tree_mesh, TreeInstance *tree_instance);
void create_terrain_mesh(Mesh **terrain_mesh, float terrain[TERRAIN_SIZE][TERRAIN_SIZE]);




int main() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Terrain", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW: %s\n", glewGetErrorString(err));
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    

const char *vertex_shader_path = "vertex_shader.glsl";
const char *fragment_shader_path = "fragment_shader.glsl";
GLuint shader_program = create_shader_program(vertex_shader_path, fragment_shader_path);





    float composition[TERRAIN_SIZE][TERRAIN_SIZE] = {0};

    // Generate terrain and forest
    float terrain[TERRAIN_SIZE][TERRAIN_SIZE];
    generate_terrain(terrain, 32.0f, composition);

    TreeInstance *trees;
    int tree_count;
    generate_forest(&trees, &tree_count, terrain, 32.0f);

    // Create terrain and tree meshes
    Mesh *terrain_mesh;
    create_terrain_mesh(&terrain_mesh, terrain);

    Mesh **tree_meshes = (Mesh **)malloc(tree_count * sizeof(Mesh *));
    for (int i = 0; i < tree_count; i++) {
        create_tree_mesh(&tree_meshes[i], &trees[i]);
    }

    // Set up camera
    Camera camera;
    vec3 camera_position = {TERRAIN_SIZE / 2.0f, TERRAIN_SIZE * 1.5f, TERRAIN_SIZE / 2.0f};
    vec3 camera_up = {0.0f, 1.0f, 0.0f};
    vec3 camera_front = {TERRAIN_SIZE / 2.0f, 0.0f, TERRAIN_SIZE / 2.0f};
    float fov = glm_rad(45.0f);
    float aspect_ratio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
    float near_plane = 0.1f;
    float far_plane = TERRAIN_SIZE * 2.0f;

    camera_init(&camera, camera_position, camera_up, camera_front, fov, aspect_ratio, near_plane, far_plane);

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        // Render the scene
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_program);

        // Set up view and projection matrices
        mat4 view, projection;
        camera_get_view_matrix(&camera, view);
        camera_get_projection_matrix(&camera, projection);

        GLint view_location = glGetUniformLocation(shader_program, "view");
        glUniformMatrix4fv(view_location, 1, GL_FALSE, (const GLfloat *)view);

        GLint projection_location = glGetUniformLocation(shader_program, "projection");
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, (const GLfloat *)projection);

        // Render terrain
        render_mesh(terrain_mesh);

        // Render trees
        for (int i = 0; i < tree_count; i++) {
            render_mesh(tree_meshes[i]);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up resources
    free_mesh(terrain_mesh);
    for (int i = 0; i < tree_count; i++) {
        free_mesh(tree_meshes[i]);
    }
    free(tree_meshes);
    glDeleteProgram(shader_program);

    glfwTerminate();

    return 0;
}

