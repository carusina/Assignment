#include <stdio.h>
#include <pthread.h>

#define NUMTHREAD 2

int flag[NUMTHREAD] = {0, 0};
int turn = 0;
int num = 1;
int max = 100;

void multi(int st) {
    printf("func%d start\n", st);
    for(int i = max/NUMTHREAD; num < max+1 && i > 0; num++, i--) {
        printf("%d * 3 = %d\n", num, num*3);
        // printf("%d  ", num);
    }
    printf("func%d end\n", st);
}

void *func0(void *arg) {
    flag[0] = 1;
    turn = 1;

    while(flag[1] == 1 && turn == 1) {}

    multi(0);
    flag[0] = 0;
    pthread_exit(NULL);
}

void *func1(void *arg) {
    flag[1] = 1;
    turn = 0;

    while(flag[0] == 1 && turn == 0) {}

    multi(1);
    flag[1] = 0;
    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, func0, NULL);
    pthread_create(&t2, NULL, func1, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}
