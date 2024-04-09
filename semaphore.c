#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

#define NUMTHREAD 4

int num = 1;
int max = 100;

sem_t semaphore;


void multi(int st) {
    printf("func%d start\n", st);
    for(int i = max/NUMTHREAD; num < max+1 && i > 0; num++, i--) {
        printf("%d * 3 = %d\t", num, num*3);
        // printf("%d  ", num);
    }
    printf("\nfunc%d end\n", st);
}


void *func0 (void *arg) {
    sem_wait(&semaphore);
    multi(0);
    sem_post(&semaphore);
    pthread_exit(NULL);
}
void *func1 (void *arg) {
    sem_wait(&semaphore);
    multi(1);
    sem_post(&semaphore);
    pthread_exit(NULL);
}
void *func2 (void *arg) {
    sem_wait(&semaphore);
    multi(2);
    sem_post(&semaphore);
    pthread_exit(NULL);
}
void *func3 (void *arg) {
    sem_wait(&semaphore);
    multi(3);
    sem_post(&semaphore);
    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2, t3, t4;
    sem_init(&semaphore, 0, 1);

    pthread_create(&t1, NULL, func0, NULL);
    pthread_create(&t2, NULL, func1, NULL);
    pthread_create(&t3, NULL, func2, NULL);
    pthread_create(&t4, NULL, func3, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
}
