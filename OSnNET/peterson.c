#include <stdio.h>
#include <pthread.h>

#define NUMTHREAD 2 // 스레드 개수

// 전역 변수
int flag[NUMTHREAD] = {0, 0}; // 0: false, 1: true 
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

void *func0(void *arg) {
    flag[0] = 1; // 플래그 올리기
    turn = 1; // 1번 스레드에게 턴 양보

    while(flag[1] == 1 && turn == 1) {} // 1번 스레드가 임계 영역을 사용하고 싶고
                                        // 1번 스레드의 턴이면 대기
    // 임계 영역 시작
    multi(0);
    // 임계 영역 종료
    flag[0] = 0; // 플래그 내리기
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
