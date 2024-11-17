#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <time.h>

#define MAX 100
#define INFINITY INT_MAX

int getInput(FILE* file, char locations[][20], int distances[][MAX]) {
    int n = 0;
    int distance;
    char location1[20], location2[20];
    for(int i = 0; i < MAX; i++) { 
        for(int j = 0; j < MAX; j++) {
            if(i == j) distances[i][j] = 0;
            else distances[i][j] = INFINITY;
        }
    }

    while(fscanf(file, "(%[^,], %[^,], %d)\n", location1, location2, &distance) == 3) {
        int dis1 = -1, dis2 = -1;

        for(int i = 0; i < n; i++) {
            if(strcmp(location1, locations[i]) == 0)    dis1 = i;
            else if(strcmp(location2, locations[i]) == 0)   dis2 = i;
        }
        if(dis1 == -1) {
            strcpy(locations[n], location1);
            dis1 = n++;
        }
        if(dis2 == -1) {
            strcpy(locations[n], location2);
            dis2 = n++;
        }
        if(distances[dis1][dis2] > distance) {
            distances[dis1][dis2] = distance;
            distances[dis2][dis1] = distance;
        }


        if (n >= MAX) {
            printf("더 이상 데이터를 저장할 수 없음\n");
            break;
        }
    }

    return n;
}

void AllPairsShortest(int distances[][MAX], int n){
    for(int k = 0; k < n; k++) {
        for(int i = 0; i < n; i++) {
            if(i != k) {
                for(int j = 0; j < n; j++) {
                    if(j != i && j != k) {
                        if(distances[i][k] != INFINITY && distances[k][j] != INFINITY) {
                            distances[i][j] = distances[i][k]+distances[k][j] < distances[i][j] ? distances[i][k]+distances[k][j] : distances[i][j];
                        }
                    }
                }
            }
        }
    }
}

void printDistances(char locations[][20], int distances[][MAX], int n) {
    printf("       ");
    for (int i = 0; i < n; i++) printf("%-10s", locations[i]);
    printf("\n");

    int k = 1;
    for (int i = 0; i < n; i++, k++) {
        printf("%-10s", locations[i]);
        for (int j = 0; j < n; j++) {
            if(j < k) printf("        ");
            else printf("%-8d", distances[i][j]);
        }
        printf("\n");
    }
}

int main() {
    char locations[MAX][20];
    int distances[MAX][MAX];
    FILE *file = fopen("input_Floyd.txt", "r");
    if (file == NULL) {
        printf("파일을 열 수 없음.\n");
        return 1;
    }

    int n = getInput(file, locations, distances);
    fclose(file);

    clock_t start = clock();
    AllPairsShortest(distances, n);
    clock_t finish = clock();

    printDistances(locations, distances, n);
    printf("running time (ms): %lu\n", finish-start);

    return 0;
}