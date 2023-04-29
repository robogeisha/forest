#include <stdlib.h>
#include "forest_environment.h"

void generate_terrain(float (*terrain)[TERRAIN_SIZE], float scale, float (*composition)[TERRAIN_SIZE]) {
    for (int i = 0; i < TERRAIN_SIZE; i++) {
        for (int j = 0; j < TERRAIN_SIZE; j++) {
            float x = (float)i / scale;
            float y = (float)j / scale;

            // Calculate the base height using Perlin noise
            float base_height = perlin_noise(x, y);

            // Use the composition value to modify the height
            float height = base_height + (composition[i][j] - 0.5f) * 2.0f;

            // Set the height of the terrain at this point
            terrain[i][j] = height;
        }
    }
}

char *create_complex_tree(float height, int tree_type) {
    LSystem tree;
    init_lsystem(&tree, 'A');

    switch (tree_type) {
        case 1:
            add_rule(&tree, 'A', "B[A]A");
            add_rule(&tree, 'B', "BB");
            break;
        case 2:
            add_rule(&tree, 'A', "F[+A]F[-A]+A");
            add_rule(&tree, 'F', "FF");
            break;
        default:
            add_rule(&tree, 'A', "F[+A]F[-A]F");
            add_rule(&tree, 'F', "FF");
            break;
    }

    char *sequence = generate_lsystem_sequence(&tree);
    free(tree.rules);
    return sequence;
}
void generate_forest(TreeInstance **trees, int *tree_count, float terrain[TERRAIN_SIZE][TERRAIN_SIZE], float density_scale) {
    *tree_count = 0;

    // Count the number of trees needed based on the density
    for (int i = 0; i < TERRAIN_SIZE; i++) {
        for (int j = 0; j < TERRAIN_SIZE; j++) {
            float density = perlin_noise((float)i / density_scale, (float)j / density_scale);
            if (density > TREE_DENSITY_THRESHOLD) {
                (*tree_count)++;
            }
        }
    }

    // Allocate memory for the trees
    *trees = (TreeInstance *)malloc(*tree_count * sizeof(TreeInstance));

    // Generate tree instances
    int tree_index = 0;
    for (int i = 0; i < TERRAIN_SIZE; i++) {
        for (int j = 0; j < TERRAIN_SIZE; j++) {
            float density = perlin_noise((float)i / density_scale, (float)j / density_scale);
            if (density > TREE_DENSITY_THRESHOLD) {
                float height = 1.0 + perlin_noise((float)i / density_scale, (float)j / density_scale) * 4.0;
                int tree_type = rand() % 3;
                char *tree_sequence = create_complex_tree(height, tree_type);

                TreeInstance instance;
                instance.position.x = (float)i;
                instance.position.y = terrain[i][j];
                instance.position.z = (float)j;
                instance.tree_sequence = tree_sequence;

                (*trees)[tree_index] = instance;
                tree_index++;
            }
        }
    }
}


