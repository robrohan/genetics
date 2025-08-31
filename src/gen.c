#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct dna
{
    // the length of the sequence including
    // the null byte
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
    // struct dna *mating_pool;
} pool;

typedef double (*fitnessfunction)(const char *, struct dna *);

/////////////////////////////////////////

struct population gpop = {};
struct pool *mating_pool = NULL;

/////////////////////////////////////////

void crossover(struct dna *partner_a, struct dna *partner_b, struct dna *child);
char random_char(void);
static void create_population(const char *target, double mutation_rate, size_t pop_max);
void calculate_fitness(struct population *pop, fitnessfunction fn);
void natural_selection(struct population *pop);
void next_generation(struct population *pop, struct pool *n_mating_pool);
void evaluate(struct population *pop);
void mutation(struct population *pop);
double fitness(const char *target, struct dna *n_dna);
void run(size_t max_iter);

/////////////////////////////////////////

char random_char(void)
{
    // 48 - 125
    // 64 - 124 is mostly A-Za-z
    float percent = (float)((double)rand() / (double)RAND_MAX);
    // 124-64 == 60
    // 125-48 == 77
    char pos = (char)(77 * percent);
    char selection = (77 + pos);
    // replace some non letter with parts chars we need
    if (selection == '[')
        selection = ' ';
    if (selection == ']')
        selection = '.';
    if (selection == '\\')
        selection = '\'';
    return selection;
}

static void create_population(const char *target, double mutation_rate, size_t pop_max)
{
    gpop.target = target;
    gpop.rate = mutation_rate;
    gpop.max = pop_max;

    size_t genelen = strlen(target) + 1;
    struct dna *ents = (struct dna *)malloc(sizeof(struct dna) * pop_max);
    gpop.entities = ents;
    for (size_t i = 0; i < pop_max; i++)
    {
        gpop.entities[i].genes = malloc(sizeof(char) * (size_t)genelen);
        for (size_t j = 0; j < genelen; j++)
        {
            gpop.entities[i].genes[j] = random_char();
        }
        // gpop.entities[i].genes[genelen] = '\0';
        gpop.entities[i].len = genelen - 1;
    }
    gpop.len = pop_max;
}

void calculate_fitness(struct population *pop, fitnessfunction fn)
{
    for (size_t p = 0; p < pop->len; p++)
    {
        double score = fn(pop->target, &pop->entities[p]);
        pop->entities[p].fitness = score;
        // printf("%s\t%f\n", pop->entities[p].genes, score);
    }
}

void natural_selection(struct population *pop)
{
    /////////
    // current highest fitness
    double top_fitness = 0.0;
    size_t mating_pool_size = 0;
    for (size_t x = 0; x < pop->len; x++)
    {
        if (pop->entities[x].fitness > top_fitness)
            top_fitness = pop->entities[x].fitness;

        mating_pool_size += (size_t)(100 * pop->entities[x].fitness);
    }
    // printf("Top Fitness Score: %f\n", top_fitness);
    // printf("Mating Pool Size : %i\n", mating_pool_size);
    /////////

    mating_pool = malloc(sizeof(struct pool));
    mating_pool->idx = calloc(mating_pool_size, sizeof(size_t));
    mating_pool->len = mating_pool_size;

    // fill the mating pool
    size_t mpool = 0;
    for (size_t x = 0; x < pop->len; x++)
    {
        size_t ins_count = (size_t)(100 * pop->entities[x].fitness);
        for (size_t y = 0; y < ins_count; y++)
        {
            mating_pool->idx[mpool] = x;
            mpool++;
        }
    }
    mating_pool->len = mpool;
}

void next_generation(struct population *pop, struct pool *n_mating_pool)
{
    // printf("Mating Pool Size : %i\n", mating_pool->len);
    struct dna *newents = (struct dna *)calloc(pop->max, sizeof(struct dna));

    for (size_t p = 0; p < pop->len; p++)
    {
        double p1 = (double)rand() / (double)RAND_MAX;
        double p2 = (double)rand() / (double)RAND_MAX;

        size_t i1 = (size_t)floor((n_mating_pool->len) * p1);
        size_t i2 = (size_t)floor((n_mating_pool->len) * p2);

        size_t idx1 = n_mating_pool->idx[i1];
        size_t idx2 = n_mating_pool->idx[i2];

        // printf("%i x %i ----- %i x %i \t (%i %f %f) \n",
        //     i1, i2, idx1, idx2,
        //     mating_pool->len, p1, p2);

        struct dna parent_a = pop->entities[idx1];
        struct dna parent_b = pop->entities[idx2];

        // printf("\t %i x %i\n", parent_a.len, parent_b.len);

        struct dna *child = calloc(1, sizeof(struct dna));
        child->genes = calloc(parent_a.len, sizeof(char));
        child->len = parent_a.len;
        crossover(&parent_a, &parent_b, child);

        newents[p] = *child;
    }

    free(n_mating_pool->idx);
    n_mating_pool->len = 0;
    n_mating_pool = NULL;
    free(n_mating_pool);

    free(pop->entities);
    pop->entities = NULL;
    pop->entities = newents;
}

void evaluate(struct population *pop)
{
    double top_fitness = 0.0;
    size_t tf = 0;
    for (size_t p = 0; p < pop->len; p++)
    {
        if (top_fitness < pop->entities[p].fitness)
        {
            top_fitness = pop->entities[p].fitness;
            tf = p;
        }
    }
    printf("%s %f\n", pop->entities[tf].genes, top_fitness);

    if (top_fitness >= 1.0)
    {
        printf("\n\n%s %f\n\n", pop->entities[tf].genes, top_fitness);
        exit(1);
    }
}

//////////////////////////////////////////////////////////

// Calculate the fitness score for a single dna strand
double fitness(const char *target, struct dna *n_dna)
{
    double score = 0;
    for (size_t i = 0; i < n_dna->len; i++)
    {
        if (n_dna->genes[i] == target[i])
        {
            score++;
        }
    }
    return score / (n_dna->len);
}

// "Mate" two partners and output into child
void crossover(struct dna *partner_a, struct dna *partner_b, struct dna *child)
{
    size_t midpoint = partner_a->len >> 1;

    if (partner_b->len <= 0)
    {
        for (size_t c = 0; c < partner_a->len; c++)
        {
            child->genes[c] = partner_a->genes[c];
        }
        child->len = partner_a->len;
        return;
    }

    for (size_t c = 0; c < partner_a->len; c++)
    {
        if (c > midpoint)
            child->genes[c] = partner_a->genes[c];
        else
            child->genes[c] = partner_b->genes[c];
    }
    // child->genes[partner_a->len] = '\0';
    child->len = partner_a->len;
}

void mutation(struct population *pop)
{
    for (size_t p = 0; p < pop->len; p++)
    {
        struct dna *child = &pop->entities[p];
        // loop over the whole sequence...
        for (size_t c = 0; c < child->len; c++)
        {
            // if the genes match the target, leave it
            if (pop->target[c] == child->genes[c])
                continue;

            // otherwise if hit the mutation rate, randomly pick
            // a new character
            double r = (double)rand() / (double)RAND_MAX;
            if (r < pop->rate)
            {
                child->genes[c] = random_char();
            }
        }
    }
}

//////////////////////////////////////////////////////////

void run(size_t max_iter)
{
    for (size_t g = 0; g < max_iter; g++)
    {
        // printf("Generation: %i\n", g);
        // calc each entity fitness
        calculate_fitness(&gpop, &fitness);
        // build the mating pool based on fitness
        natural_selection(&gpop);
        //
        next_generation(&gpop, mating_pool);
        //
        mutation(&gpop);
        //
        calculate_fitness(&gpop, &fitness);
        //
        evaluate(&gpop);
    }
}

//////////////////////////////////////////////////////////

int main(void)
{
    printf("~~ Howdy ~~\n");

    // srand(42);
    srand((unsigned int)time(NULL));
    const char *target = "Here's to the crazy ones. The misfits. The rebels.";
    create_population(target, 0.03, (size_t)625);
    printf("\n");
    printf("Pop Pointer: %p\n", &gpop);
    printf("[0] Fitness: %f\n", gpop.entities[0].fitness);
    printf("[0]   Genes: \"%s\" \t (%lu)\n", gpop.entities[0].genes, strlen(gpop.entities[0].genes));
    printf("     Target: \"%s\" \t (%lu)\n", target, strlen(target));
    printf("\n...\n");

    run(10000);

    printf("\nDone.\n");
    return 1;
}
