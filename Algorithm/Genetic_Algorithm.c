#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define CANDIDATE_SIZE 8
#define NUM_VERTEX 8
#define GENERATIONS 1000
#define CROSSOVER_RATIO 1.0
#define MUTATION_RATIO 0.01

typedef struct {
    int x, y;
} Vertex;

Vertex vertecies[NUM_VERTEX] = {
    {0, 3}, {7, 5}, {6, 0}, {4, 3}, {1, 0}, {5, 3}, {2, 2}, {4, 1}
};

typedef struct {
    int path[NUM_VERTEX+1];
    double fitness;
} Indiviual;

Indiviual candidate[CANDIDATE_SIZE];
Indiviual new_candidate[CANDIDATE_SIZE];

double calculate_distance(Vertex a, Vertex b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

double calculate_path_length(int *path) {
    double total_distance = 0;
    for(int i = 0; i < NUM_VERTEX; i++) {
        total_distance += calculate_distance(vertecies[path[i]], vertecies[path[i+1]]);
    }

    return total_distance;
}

void initialize_candidate() {
    for(int i = 0; i < CANDIDATE_SIZE; i++) {
        for(int j = 1; j < NUM_VERTEX; j++) {
            candidate[i].path[j] = j;
        }
        candidate[i].path[NUM_VERTEX] = 0;

        for(int j = NUM_VERTEX-1; j > 0; j--) {
            int rand_index = (rand() % j) + 1;
            int temp = candidate[i].path[j];
            candidate[i].path[j] = candidate[i].path[rand_index];
            candidate[i].path[rand_index] = temp;
        }
        candidate[i].fitness = 1.0 / calculate_path_length(candidate[i].path);

        // for (int j = 0; j < NUM_VERTEX+1; j++) {
        //     printf("%c ", 'A' + candidate[i].path[j]);
        // }
        // printf("\n");
    }
}

Indiviual select_parent() {
    double totla_fitness = 0;
    for(int i = 0; i < CANDIDATE_SIZE; i++) {
        totla_fitness += candidate[i].fitness;
    }

    double rand_value = ((double) rand() / RAND_MAX) * totla_fitness;
    double running_sum = 0;

    for(int i = 0; i < CANDIDATE_SIZE; i++) {
        running_sum += candidate[i].fitness;
        if(running_sum >= rand_value) {
            return candidate[i];
        }
    }

    return candidate[CANDIDATE_SIZE-1];
}

void cross_over(int *parent1, int *parent2, int *child1, int *child2) {
    int rand_index1 = (rand() % (NUM_VERTEX-1)) + 1;
    int rand_index2 = (rand() % (NUM_VERTEX-1)) + 1;
    if(rand_index1 > rand_index2) {
        int temp = rand_index1;
        rand_index1 = rand_index2;
        rand_index2 = temp;
    }

    int visited1[NUM_VERTEX] = {0};
    int visited2[NUM_VERTEX] = {0};

    for(int i = rand_index1; i <= rand_index2; i++) {
        child1[i] = parent2[i];
        visited1[child1[i]] = 1;

        child2[i] = parent1[i];
        visited2[child2[i]] = 1;
    }

    int index1 = 0, index2 = 0;
    for(int i = 0; i < NUM_VERTEX; i++) {
        if(!visited1[parent1[i]]) {
            while(index1 >= rand_index1 && index1 <= rand_index2) index1++;
            child1[index1++] = parent1[i];
        }
        if(!visited2[parent2[i]]) {
            while(index2 >= rand_index1 && index2 <= rand_index2) index2++;
            child2[index2++] = parent2[i];
        }
    }

    child1[NUM_VERTEX] = child2[NUM_VERTEX] = 0;
}


void mutate(int *path) {
    if((double) rand() / RAND_MAX < MUTATION_RATIO) {
        int i = (rand() % (NUM_VERTEX-1)) + 1;
        int j = (rand() % (NUM_VERTEX-1)) + 1;
        int temp = path[i];
        path[i] = path[j];
        path[j] = temp;
    }
}

void GeneticAlgorithm() {
    for(int i = 0; i < CANDIDATE_SIZE; i++) {
        Indiviual parent1 = select_parent();
        Indiviual parent2 = select_parent();

        if(((double) rand() / RAND_MAX) < CROSSOVER_RATIO) {
            cross_over(parent1.path, parent2.path, new_candidate[i].path, new_candidate[i+1].path);
        } else {
            for(int j = 0; j < NUM_VERTEX; j++) {
                new_candidate[i].path[j] = parent1.path[j];
            }
        }
        mutate(new_candidate[i].path);
        new_candidate[i].fitness = 1.0 / calculate_path_length(new_candidate[i].path);
    }

    for(int i = 0; i < CANDIDATE_SIZE; i++) {
        for(int j = 0; j < NUM_VERTEX+1; j++) {
            candidate[i].path[j] = new_candidate[i].path[j];
        }
        candidate[i].fitness = new_candidate[i].fitness;
    }
}

void print_best_solution() {
    int best_index = 0;
    for(int i = 0; i < CANDIDATE_SIZE; i++) {
        if (candidate[i].fitness > candidate[best_index].fitness) {
            best_index = i;
        }
    }

    printf("Best Path: ");
    for(int i = 0; i < NUM_VERTEX+1; i++) {
        printf("%c ", 'A' + candidate[best_index].path[i]);
    }
    printf("\nDistance: %.2f\n", 1.0 / candidate[best_index].fitness);
}

int main() {
    srand(time(NULL));
    initialize_candidate();

    for(int generation = 0; generation < GENERATIONS; generation++) {
        GeneticAlgorithm();
    }

    print_best_solution();

    return 0;
}