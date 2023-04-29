#include "lsystem.h"

// Initialize an L-System with the given axiom
void init_lsystem(LSystem *lsystem, char axiom) {
    lsystem->axiom = axiom;
    lsystem->rules = NULL;
    lsystem->rule_count = 0;
    lsystem->depth = MAX_DEPTH;
}

// Add a rule to the L-System
void add_rule(LSystem *lsystem, char predecessor, const char *successor) {
    lsystem->rule_count++;
    lsystem->rules = (Rule *)realloc(lsystem->rules, lsystem->rule_count * sizeof(Rule));
    lsystem->rules[lsystem->rule_count - 1].predecessor = predecessor;
    lsystem->rules[lsystem->rule_count - 1].successor = successor;
}

// Apply the L-System rules to a string
static char *apply_rules(LSystem *lsystem, const char *input) {
    size_t output_size = 1; // for the null terminator
    char *output = NULL;
    const char *current = input;

    while (*current) {
        for (int i = 0; i < lsystem->rule_count; i++) {
            if (*current == lsystem->rules[i].predecessor) {
                size_t len = strlen(lsystem->rules[i].successor);
                output_size += len;
                output = (char *)realloc(output, output_size);
                strcat(output, lsystem->rules[i].successor);
                break;
            }
        }

        current++;
    }

    return output;
}

// Generate the L-System sequence for the given depth
char *generate_lsystem_sequence(LSystem *lsystem) {
    char *current_sequence = (char *)calloc(2, sizeof(char));
    current_sequence[0] = lsystem->axiom;

    for (int i = 0; i < lsystem->depth; i++) {
        char *next_sequence = apply_rules(lsystem, current_sequence);
        free(current_sequence);
        current_sequence = next_sequence;
    }

    return current_sequence;
}
