#ifndef SIMULATOR_CREATURE_H_
#define SIMULATOR_CREATURE_H_

#include "common.h"
#include "genome.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(THREADED) && THREADED == 1
    #include "tsqueue/tsqueue.h"
#endif

struct environment;
typedef enum { SPECIES_A } creature_species_t;

#define CREATURE_SIZE 192
#define CREATURE_PADDING_SIZE                                                  \
    (CREATURE_SIZE - sizeof(genome_t) - sizeof(creature_species_t) - 1)
struct creature {
    genome_t genome;
    creature_species_t species;
    struct {
        uint8_t alive : 1;
    } state;
    uint8_t padding[CREATURE_PADDING_SIZE];
};
_Static_assert(sizeof(struct creature) == CREATURE_SIZE, "");

void creature_init(struct creature*, creature_species_t, size_t n_genes);

#if defined(THREADED) && THREADED == 1
struct creature_workqueue_elm {
    struct creature* creature;
    struct environment* env;
    size_t grid_idx;
    creature_action_t action;
};
void creature_tick(
    struct creature*,
    struct environment*,
    size_t grid_idx,
    grid_state_t,
    creature_action_t*, size_t);
#else
creature_action_t creature_tick(
    struct creature*,
    struct environment*,
    size_t grid_idx,
    grid_state_t);
#endif
void creature_apply_action(
    struct creature*,
    struct environment*,
    size_t grid_idx,
    creature_action_t);

void creature_mutate(struct creature*, float chance);

bool creature_is_dead(struct creature*);
bool creature_is_alive(struct creature*);
void creature_kill(struct creature*);

#endif
