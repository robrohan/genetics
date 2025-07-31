#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct dna
{
    // the length of the sequence including
    // the null byte
    int len;
    float fitness;
    char *genes;
} dna;

struct population
{
    const char *target;
    float rate;
    int max;
    struct dna *entities;
    int len;
} population;

struct pool
{
    int len;
    int *idx;
    // struct dna *mating_pool;
} pool;

typedef float (*fitnessfunction)(const char *, struct dna *);

void crossover(struct dna *partner_a, struct dna *partner_b, struct dna *child);

/////////////////////////////////////////

struct population gpop = {};
struct pool *mating_pool = NULL;

/////////////////////////////////////////

char random_char()
{
    // 64 - 124 is mostly A-Za-z
    float percent = (double)rand() / (double)RAND_MAX;
    // 124-64 == 60
    char pos = (char)60 * percent;
    char selection = 60 + pos;
    // replace some non letter with parts chars we need
    if (selection == '[')
        selection = ' ';
    if (selection == ']')
        selection = '.';
    if (selection == '\\')
        selection = '\'';
    return selection;
}

void create_population(const char *target, float mutation_rate, int pop_max)
{
    gpop.target = target;
    gpop.rate = mutation_rate;
    gpop.max = pop_max;

    int genelen = strlen(target) + 1;
    struct dna *ents = (struct dna *)malloc(sizeof(struct dna) * pop_max);
    gpop.entities = ents;
    for (int i = 0; i < pop_max; i++)
    {
        gpop.entities[i].genes = malloc(sizeof(char) * genelen);
        for (int j = 0; j < genelen; j++)
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
    for (int p = 0; p < pop->len; p++)
    {
        float score = fn(pop->target, &pop->entities[p]);
        pop->entities[p].fitness = score;
        // printf("%s\t%f\n", pop->entities[p].genes, score);
    }
}

void natural_selection(struct population *pop)
{
    /////////
    // current highest fitness
    float top_fitness = 0.0;
    int mating_pool_size = 0;
    for (int x = 0; x < pop->len; x++)
    {
        if (pop->entities[x].fitness > top_fitness)
            top_fitness = pop->entities[x].fitness;

        mating_pool_size += 100 * pop->entities[x].fitness;
    }
    // printf("Top Fitness Score: %f\n", top_fitness);
    printf("Mating Pool Size : %i\n", mating_pool_size);
    /////////

    mating_pool = malloc(sizeof(struct pool));
    mating_pool->idx = calloc(mating_pool_size, sizeof(int));
    mating_pool->len = mating_pool_size;

    // fill the mating pool
    int mpool = 0;
    for (int x = 0; x < pop->len; x++)
    {
        int ins_count = 100 * pop->entities[x].fitness;
        for (int y = 0; y < ins_count; y++)
        {
            mating_pool->idx[mpool] = x;
            mpool++;
        }
    }
    mating_pool->len = mpool;
}

void next_generation(struct population *pop, struct pool *mating_pool)
{
    // printf("Mating Pool Size : %i\n", mating_pool->len);
    struct dna *newents = (struct dna *)calloc(pop->max, sizeof(struct dna));

    for (int p = 0; p < pop->len; p++)
    {
        float p1 = (double)rand() / (double)RAND_MAX;
        float p2 = (double)rand() / (double)RAND_MAX;

        int i1 = floor((mating_pool->len) * p1);
        int i2 = floor((mating_pool->len) * p2);

        int idx1 = mating_pool->idx[i1];
        int idx2 = mating_pool->idx[i2];

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

    free(mating_pool->idx);
    mating_pool->len = 0;
    mating_pool = NULL;
    free(mating_pool);

    free(pop->entities);
    pop->entities = NULL;
    pop->entities = newents;
}

void evaluate(struct population *pop)
{
    float top_fitness = 0.0;
    int tf = 0;
    for (int p = 0; p < pop->len; p++)
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
float fitness(const char *target, struct dna *dna)
{
    float score = 0;
    for (int i = 0; i < dna->len; i++)
    {
        if (dna->genes[i] == target[i])
        {
            score++;
        }
    }
    return score / (dna->len);
}

// "Mate" two partners and output into child
void crossover(struct dna *partner_a, struct dna *partner_b, struct dna *child)
{
    int midpoint = partner_a->len >> 1;

    if (partner_b->len <= 0)
    {
        for (int c = 0; c < partner_a->len; c++)
        {
            child->genes[c] = partner_a->genes[c];
        }
        child->len = partner_a->len;
        return;
    }

    for (int c = 0; c < partner_a->len; c++)
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
    for (int p = 0; p < pop->len; p++)
    {
        struct dna *child = &pop->entities[p];
        for (int c = 0; c < child->len; c++)
        {
            if (pop->target[c] == child->genes[c])
                continue;

            float r = (double)rand() / (double)RAND_MAX;
            if (r <= pop->rate)
            {
                child->genes[c] = random_char();
            }
        }
    }
}

//////////////////////////////////////////////////////////

void run(int max_iter)
{
    for (int g = 0; g < max_iter; g++)
    {
        printf("Generation: %i\n", g);
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

int main()
{
    printf("~~ Howdy ~~\n");

    // srand(42);
    const char *target = "Here's to the crazy ones. The misfits. The rebels.";
    create_population(target, 0.001, 450);
    printf("\n");
    printf("Pop Pointer: %p\n", &gpop);
    printf("[0] Fitness: %f\n", gpop.entities[0].fitness);
    printf("[0]   Genes: \"%s\" \t (%lu)\n", gpop.entities[0].genes, strlen(gpop.entities[0].genes));
    printf("     Target: \"%s\" \t (%lu)\n", target, strlen(target));
    printf("\n...\n");

    run(80000);

    printf("\nDone.\n");
    return 1;
}
