#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

Sem_t semaphore;

#define NUMTHREAD 4

int sema = 1;
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
    if(sema != 0) {
        sema--;
        sem_wait(&semaphore);
        multi(0);
        sema++;
        sem_post(&semaphore);
    }
    else {
        while(sema == 0) {}
        sema--;
        sem_wait(&semaphore);
        multi(0);
        sema++;
        sem_post(&semaphore);
    }
}

void *func1(void *arg) {
    if(sema != 0) {
        sema--;
        sem_wait(&semaphore);
        multi(1);
        sema++;
        sem_post(&semaphore);
    }
    else {
        while(sema == 0) {}
        sema--;
        sem_wait(&semaphore);
        multi(1);
        sema++;
        sem_post(&semaphore);
    }
}

void *func2(void *arg) {
    if(sema != 0) {
        sema--;
        sem_wait(&semaphore);
        multi(2);
        sema++;
        sem_post(&semaphore);
    }
    else {
        while(sema == 0) {}
        sema--;
        sem_wait(&semaphore);
        multi(2);
        sema++;
        sem_post(&semaphore);
    }
}

void *func3(void *arg) {
    if(sema != 0) {
        sema--;
        sem_wait(&semaphore);
        multi(3);
        sema++;
        sem_post(&semaphore);
    }
    else {
        while(sema == 0) {}
        sema--;
        sem_wait(&semaphore);
        multi(3);
        sema++;
        sem_post(&semaphore);
    }
}

int main() {
    pthread_t t1, t2, t3, t4;

    pthread_create(&t1, NULL, func0, NULL);
    pthread_create(&t2, NULL, func1, NULL);
    pthread_create(&t3, NULL, func2, NULL);
    pthread_create(&t4, NULL, func3, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
}
