#ifndef PERLIN_H
#define PERLIN_H

#include <math.h>
#include <stdlib.h>

static int p[512];

static void generate_permutation_table(int p[512]) {
    int permutation[256];

    for (int i = 0; i < 256; i++) {
        permutation[i] = i;
    }

    for (int i = 0; i < 256; i++) {
        int j = rand() % 256;
        int temp = permutation[i];
        permutation[i] = permutation[j];
        permutation[j] = temp;
    }

    for (int i = 0; i < 256; i++) {
        p[256 + i] = p[i] = permutation[i];
    }
}

static float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

static float lerp(float t, float a, float b) {
    return a + t * (b - a);
}

static float grad(int hash, float x, float y) {
    int h = hash & 3;
    float u = h < 2 ? x : y;
    float v = h < 1 ? y : x;
    return ((h & 1) ? -u : u) + ((h & 2) ? -2.0 * v : 2.0 * v);
}

static float perlin_noise(float x, float y) {
    int X = (int)floor(x) & 255;
    int Y = (int)floor(y) & 255;

    x -= floor(x);
    y -= floor(y);

    float u = fade(x);
    float v = fade(y);

    int A = p[X] + Y, AA = p[A], AB = p[A + 1];
    int B = p[X + 1] + Y, BA = p[B], BB = p[B + 1];

    return lerp(v, lerp(u, grad(p[AA], x, y),
                        grad(p[BA], x - 1, y)),
                lerp(u, grad(p[AB], x, y - 1),
                     grad(p[BB], x - 1, y - 1)));
}

#endif // PERLIN_H
