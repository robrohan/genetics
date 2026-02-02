#include "gen.h"

struct population gpop = {};
struct pool *mating_pool = NULL;

void run(size_t max_iter)
{
    for (size_t g = 0; g < max_iter; g++)
    {
        mating_pool = malloc(sizeof(struct pool));

        calculate_fitness(&gpop, &fitness);
        natural_selection(&gpop, mating_pool);
        next_generation(&gpop, mating_pool);
        mutation(&gpop);
        calculate_fitness(&gpop, &fitness);
        evaluate(&gpop);

        free(mating_pool);
    }
}

int main(void)
{
    srand((unsigned int)time(NULL));

    const char *target = "Here's to the crazy ones. The misfits. The rebels.";

    create_population(target, 0.01, (size_t)625, &gpop);

    printf("\n");
    printf("Pop Pointer: %p\n", &gpop);
    printf("[0] Fitness: %f\n", gpop.entities[0].fitness);
    printf("[0]   Genes: \"%s\" \t (%lu)\n", gpop.entities[0].genes, strlen(gpop.entities[0].genes));
    printf("     Target: \"%s\" \t (%lu)\n", target, strlen(target));
    printf("\n...\n");

    run(50000);

    printf("\nDone.\n");
    return 0;
}
