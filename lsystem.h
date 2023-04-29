#ifndef LSYSTEM_H
#define LSYSTEM_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// Maximum depth of the L-System
#define MAX_DEPTH 10

// A structure representing a single rule in the L-System
typedef struct {
    char predecessor;
    const char *successor;
} Rule;

// A structure representing an L-System
typedef struct {
    char axiom;
    Rule *rules;
    int rule_count;
    int depth;
} LSystem;

// Function prototypes
void init_lsystem(LSystem *lsystem, char axiom);
void add_rule(LSystem *lsystem, char predecessor, const char *successor);
char *generate_lsystem_sequence(LSystem *lsystem);

#endif // LSYSTEM_H
