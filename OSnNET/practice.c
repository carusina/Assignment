#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

#define NUMTHREAD 4

int num = 1;
int max = 100;

sem_t semaphore;    

void *func0 (void *arg) {
    sem_wait(&semaphore);
    printf("func0 started\n");
    for(int i = max/NUMTHREAD; num < max+1 && i > 0; num++, i--) {
        printf("%d * 3 = %d\n", num, num*3);
    }
    printf("func0 ended\n");
    sem_post(&semaphore);
    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2, t3, t4;
    sem_init(&semaphore, 0, 0);

    pthread_create(&t1, NULL, func0, NULL);
    pthread_create(&t2, NULL, func0, NULL);
    pthread_create(&t3, NULL, func0, NULL);
    pthread_create(&t4, NULL, func0, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
}
