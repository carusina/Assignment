#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define MAX 100
#define INFINITY INT_MAX

typedef struct {
    char location1[20];
    char location2[20];
    int distance;
} Route;

int is_Unique(Route Distance[], char* location, int cnt) {
    for(int i = 0; i < cnt; i++) {
        if(strcmp(Distance[i].location2, location) == 0) return 0;
    }
    return 1;
}

int is_Unique_Locations(char Locations[][20], char* location, int cnt) {
    for(int i = 0; i < cnt; i++) {
        if(strcmp(Locations[i], location) == 0) return 0;
    }
    return 1;
}

int getDistance(Route routes[], int n, char org[], char dest[]) {
    for(int i = 0; i < n; i++) {
        if(!(strcmp(routes[i].location1, org) || strcmp(routes[i].location2, dest))) {
            return routes[i].distance;
        }
        else if(!(strcmp(routes[i].location2, org) || strcmp(routes[i].location1, dest))) {
            return routes[i].distance;
        }
    }
    return INFINITY;
}

void updateDistance(Route routes[], int n, Route Distance[], int cnt, char org[], int min_distance) {
    for(int i = 0; i < cnt; i++) {
        int dis = getDistance(routes, n, org, Distance[i].location2);
        if(dis < INFINITY) dis += min_distance;

        if(dis < Distance[i].distance) Distance[i].distance = dis;
    }
}

Route* ShortestPath(Route routes[], int n, char start[], int* cnt) {
    Route* Distance = (Route*)malloc(n * sizeof(Route));
    int visited[MAX] = {0};
    
    for(int i = 0; i < n; i++) {
        if(is_Unique(Distance, routes[i].location1, *cnt)) {
            strcpy(Distance[*cnt].location1, start);
            strcpy(Distance[*cnt].location2, routes[i].location1);
            Distance[*cnt].distance = INFINITY;
            (*cnt)++;
        }
        else if(is_Unique(Distance, routes[i].location2, *cnt)) {
            strcpy(Distance[*cnt].location1, start);
            strcpy(Distance[*cnt].location2, routes[i].location2);
            Distance[*cnt].distance = INFINITY;
            (*cnt)++;
        }
    }
    for (int i = 0; i < *cnt; i++) {
        if (!strcmp(start, Distance[i].location2)) {
            Distance[i].distance = 0;
            break;
        }
    }

    for (int i = 0; i < *cnt; i++) {
        int min_dis = INFINITY;
        int min_index = -1;

        for(int j = 0; j < *cnt; j++) {
            if(!visited[j] && Distance[j].distance < min_dis) {
                min_dis = Distance[j].distance;
                min_index = j;
            }
        }
        if(min_index == -1) break;
        visited[min_index] = 1;
        
        updateDistance(routes, n, Distance, *cnt, Distance[min_index].location2, Distance[min_index].distance);
    }

    return Distance;
}

void printDistances(char Locations[][20], int distances[][MAX], int cnt_locations) {
    printf("       ");
    for (int i = 0; i < cnt_locations; i++) printf("%-10s", Locations[i]);
    printf("\n");

    int k = 1;
    for (int i = 0; i < cnt_locations; i++, k++) {
        printf("%-10s", Locations[i]);
        for (int j = 0; j < cnt_locations; j++) {
            if(j < k) printf("        ");
            else printf("%-8d", distances[i][j]);
        }
        printf("\n");
    }
}

int main() {
    Route routes[MAX];
    int n = 0;

    FILE *file = fopen("input_Dijkstra.txt", "r");
    if (file == NULL) {
        printf("파일을 열 수 없음.\n");
        return 1;
    }

    while (fscanf(file, "(%[^,], %[^,], %d)\n", routes[n].location1, routes[n].location2, &routes[n].distance) == 3) {
        n++;
        if (n >= MAX) {
            printf("더 이상 데이터를 저장할 수 없음\n");
            break;
        }
    }
    fclose(file);

    char Locations[MAX][20];
    int cnt_locations = 0;
    int distances[MAX][MAX];

    for(int i = 0; i < n; i++) {
        if(is_Unique_Locations(Locations, routes[i].location1, cnt_locations)) {
            strcpy(Locations[cnt_locations++], routes[i].location1);
        }
        else if(is_Unique_Locations(Locations, routes[i].location2, cnt_locations)) {
            strcpy(Locations[cnt_locations++], routes[i].location2);
        }
    }
    
    clock_t running_time = 0;
    for(int i = 0; i < cnt_locations; i++) {
        int cnt = 0;
        clock_t start = clock();
        Route* shortestPath = ShortestPath(routes, n, Locations[i], &cnt);
        clock_t finish = clock();
        running_time += (finish-start);

        
        for(int j = 0; j < cnt_locations; j++) {
            distances[i][j] = INFINITY;
        }
        
        for(int j = 0; j < cnt; j++) {
            for(int k = 0; k < cnt_locations; k++) {
                if(!strcmp(shortestPath[j].location2, Locations[k])) {
                    distances[i][k] = shortestPath[j].distance;
                    break;
                }
            }
        }
        free(shortestPath);
    }
    printDistances(Locations, distances, cnt_locations);
    printf("running time (ms): %lu\n", running_time);

    return 0;
}