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
    // 임계 영역 진입 시도
    flag[0] = 1; // 플래그 올리기
    while(flag[1] == 1) { // 1번 스레드도 임계 영역에 들어가고 싶으면 반복
        if(turn == 1) { // 0번 스레드의 턴이 아니면
            flag[0] = 0; // 플래그 내리고
            while(turn == 1) {} // 1번 스레드가 끝날 때 까지 대기
            flag[0] = 1; // 플래그 올리기
        }
    }
    // 임계 영역 시작
    multi(0);
    // 임계 영역 종료
    turn = 1; // 1번 스레드에게 턴 양보
    flag[0] = 0; // 플래그 내리기
    pthread_exit(NULL);
}

void *func1(void *arg) {
    flag[1] = 1;
    while(flag[0] == 1) {
        if(turn == 0) {
            flag[1] = 0;
            while(turn == 0) {}
            flag[1] = 1;
        }
    }

    multi(1);
    turn = 0;
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
