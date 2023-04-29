#ifndef RECEIVER_H
#define RECEIVER_H

#include "forest_environment.h"

typedef struct {
    Vec3 position;
} Receiver;

void init_receiver(Receiver *receiver, float terrain[TERRAIN_SIZE][TERRAIN_SIZE]);
void move_receiver(Receiver *receiver, float terrain[TERRAIN_SIZE][TERRAIN_SIZE], TreeInstance *trees, int tree_count);

#endif // RECEIVER_H
