#ifndef FOREST_ENVIRONMENT_H
#define FOREST_ENVIRONMENT_H

#include "perlin.h"
#include "lsystem.h"

#define TERRAIN_SIZE 512
#define TREE_DENSITY_THRESHOLD 0.6

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    Vec3 position;
    char *tree_sequence;
} TreeInstance;


void generate_terrain(float (*terrain)[TERRAIN_SIZE], float scale, float (*composition)[TERRAIN_SIZE]);
void generate_forest(TreeInstance **trees, int *tree_count, float terrain[TERRAIN_SIZE][TERRAIN_SIZE], float density_scale);

#endif // FOREST_ENVIRONMENT_H
