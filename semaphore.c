#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semaphore;

#define NUMTHREAD 4

int sema = 1;
int num = 1;
int max = 100;

void multi(int st) {
    for(int i = max/NUMTHREAD; num < max+1 && i > 0; num++, i--) {
        printf("func%d: %d * 3 = %d\n", st, num, num*3);
        // printf("%d  ", num);
    }
}

void *func0(void *arg) {
    if(sema != 0) {
        sem_wait(&semaphore);
        sema--;
        multi(0);
        sema++;
        sem_post(&semaphore);
    }
    else {
        while(sema == 0) {}
        sem_wait(&semaphore);
        sema--;
        multi(0);
        sema++;
        sem_post(&semaphore);
    }
}

void *func1(void *arg) {
    if(sema != 0) {
        sem_wait(&semaphore);
        sema--;
        multi(1);
        sema++;
        sem_post(&semaphore);
    }
    else {
        while(sema == 0) {}
        sem_wait(&semaphore);
        sema--;
        multi(1);
        sema++;
        sem_post(&semaphore);
    }
}

void *func2(void *arg) {
    if(sema != 0) {
        sem_wait(&semaphore);
        sema--;
        multi(2);
        sema++;
        sem_post(&semaphore);
    }
    else {
        while(sema == 0) {}
        sem_wait(&semaphore);
        sema--;
        multi(2);
        sema++;
        sem_post(&semaphore);
    }
}

void *func3(void *arg) {
    if(sema != 0) {
        sem_wait(&semaphore);
        sema--;
        multi(3);
        sema++;
        sem_post(&semaphore);
    }
    else {
        while(sema == 0) {}
        sem_wait(&semaphore);
        sema--;
        multi(3);
        sema++;
        sem_post(&semaphore);
    }
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
