#include <stdio.h>
#include <pthread.h>

#define NUMTHREAD 4 // 스레드 개수

int flag[NUMTHREAD] = {-1, -1, -1, -1}; // -1:idle / 0:wait-in / 1:in-CS 
int turn = 0; // 어떤 스레드의 턴인지
int num = 1;
int max = 100;

// 3의 배수 출력
void multi(int st) {
    printf("func%d start\n", st);
    for(int i = max/NUMTHREAD; num < max+1 && i > 0; num++, i--) {
        printf("%d * 3 = %d\t", num, num*3);
    }
    printf("\nfunc%d end\n", st);
}

void *func0 (void *arg) {
    while(1) {
        flag[0] = 0; // wait-in 상태
        while(turn != 0) { // 0번 스레드의 턴이 아니면
            if(flag[turn] == -1) { // 현재 턴인 스레드의 플래그가 idel이면
                turn = 0; // 0번 스레드의 턴으로 변경
            }
        }

        flag[0] = 1; // in-CS 상태
        int i = 0;
        while(i < NUMTHREAD && (i == 0 || flag[i] != 1)) { // in-CS 상태인
            i++;                                           // 다른 스레드가 있는지 확인
        }
        if(i >= NUMTHREAD) { // in-CS 상태의 스레드가 0번 스레드 밖에 없다면
                break;       // 임계 영역 진입
        }
    }
    // 임계 영역 시작
    multi(0);
    // 임계 영역 종료
    flag[0] = -1; // idle 상태
    pthread_exit(NULL);
}

void *func1 (void *arg) {
    while(1) {
        flag[1] = 0;
        while(turn != 1) {
            if(flag[turn] == -1) {
                turn = 1;
            }
        }

        flag[1] = 1;
        int i = 0;
        while(i < NUMTHREAD && (i == 1 || flag[i] != 1)) {
            i++;
        }
        if(i >= NUMTHREAD) {
                break;
        }
    }
    
    multi(1);
    flag[1] = -1;
    pthread_exit(NULL);
}

void *func2 (void *arg) {
    while(1) {
        flag[2] = 0;
        while(turn != 2) {
            if(flag[turn] == -1) {
                turn = 2;
            }
        }

        flag[2] = 1;
        int i = 0;
        while(i < NUMTHREAD && (i == 2 || flag[i] != 1)) {
            i++;
        }
        if(i >= NUMTHREAD) {
                break;
        }
    }
    
    multi(2);
    flag[2] = -1;
    pthread_exit(NULL);
}

void *func3 (void *arg) {
    while(1) {
        flag[3] = 0;
        while(turn != 3) {
            if(flag[turn] == -1) {
                turn = 3;
            }
        }

        flag[3] = 1;
        int i = 0;
        while(i < NUMTHREAD && (i == 3 || flag[i] != 1)) {
            i++;
        }
        if(i >= NUMTHREAD) {
                break;
        }
    }
    
    multi(3);
    flag[3] = -1;
    pthread_exit(NULL);
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
