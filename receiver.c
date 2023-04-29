#include <stdlib.h>
#include <stdio.h>
#include "receiver.h"

void init_receiver(Receiver *receiver, float terrain[TERRAIN_SIZE][TERRAIN_SIZE]) {
    receiver->position.x = TERRAIN_SIZE / 2;
    receiver->position.y = terrain[(int)receiver->position.x][(int)receiver->position.z] + 2;
    receiver->position.z = TERRAIN_SIZE / 2;
}

int is_tree_nearby(Vec3 position, TreeInstance *trees, int tree_count) {
    float distance_threshold = 5.0f;
    for (int i = 0; i < tree_count; i++) {
        float dx = trees[i].position.x - position.x;
        float dy = trees[i].position.y - position.y;
        float dz = trees[i].position.z - position.z;
        float distance = sqrtf(dx * dx + dy * dy + dz * dz);

        if (distance < distance_threshold) {
            return 1;
        }
    }
    return 0;
}

void move_receiver(Receiver *receiver, float terrain[TERRAIN_SIZE][TERRAIN_SIZE], TreeInstance *trees, int tree_count) {
    // Define movement vector
    Vec3 movement = {1, 0, 1};

    // Calculate new position
    Vec3 new_position = {
        receiver->position.x + movement.x,
        receiver->position.y + movement.y,
        receiver->position.z + movement.z
    };

    // Check if new position is on terrain
    if (new_position.x < 0 || new_position.x >= TERRAIN_SIZE || new_position.z < 0 || new_position.z >= TERRAIN_SIZE) {
        return;
    }

    // Update the receiver's y-coordinate based on the terrain height
    new_position.y = terrain[(int)new_position.x][(int)new_position.z] + 2;

    // Check if there's a tree nearby
    if (is_tree_nearby(new_position, trees, tree_count)) {
        // Handle tree interaction
        // For example, print a message or perform some other action
        printf("Tree encountered!\n");
    }

    // Update receiver's position
    receiver->position = new_position;
}
