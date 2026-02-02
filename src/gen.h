#ifndef H_GEN_GEN
#define H_GEN_GEN

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct dna
{
    size_t len;
    double fitness;
    char *genes;
} dna;

struct population
{
    const char *target;
    double rate;
    size_t max;
    struct dna *entities;
    size_t len;
} population;

struct pool
{
    size_t len;
    size_t *idx;
} pool;

typedef double (*fitnessfunction)(const char *, struct dna *);

void crossover(struct dna *partner_a, struct dna *partner_b, struct dna *child);
char random_char(void);
void create_population(const char *target, double mutation_rate, size_t pop_max, struct population *gpop);
void calculate_fitness(struct population *pop, fitnessfunction fn);
void natural_selection(struct population *pop, struct pool *mating_pool);
void next_generation(struct population *pop, struct pool *n_mating_pool);
void evaluate(struct population *pop);
void mutation(struct population *pop);
double fitness(const char *target, struct dna *n_dna);
void run(size_t max_iter);

#endif
