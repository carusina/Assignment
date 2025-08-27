#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>

// 프로세스 생성 개수
int N = 4;
int cnt_p = -1;

int num = 25;
int muls[] = {3, 5, 7, 9};

// 각 스레드에서 실행할 작업
void *func(void *input) {
    int cnt = (int)input;

    for(int i = 1; i < num+1; i++) {
        printf("%d * %d = %d\n", i+(num*cnt), muls[cnt_p], (i+num*cnt)*muls[cnt_p]);
    }
    pthread_exit(NULL);
}

int main() {
    // wait 인자
    int status;
    pid_t pid;
    pthread_t tid[4][4]; // thread ID 저장할 배열

    clock_t start = clock();    // 프로그램 수행 시간 측정
    for(int i = 0; i < N; i++) {
        // 프로세스 생성
        pid = fork();
        cnt_p++; // 프로세서 카운트
        
        int cnt_t = 0; // 각 프로세서의 스레트 카운트 
        if(pid == 0) {
            printf("%d번째 프로세스 생성\n", cnt_p+1);
            for(int j = 0; j < N; j++) {
                pthread_create(&tid[i][j], NULL, func, (void *)cnt_t);
                cnt_t++;
                printf("%d번째 프로세스의 %d번째 스레드 생성\n", cnt_p+1, cnt_t);
            }
            // 해당 스레드가 끝날 때 까지 대기
            for(int j = 0; j < N; j++) {
                pthread_join(tid[i][j], NULL);
            }
            break;
        }
    }

    // 메인 프로세서이면 수행
    if(pid != 0) {
        while(wait(&status) != pid) continue; // 자식 프로세스가 끝날 때 까지 대기
        clock_t end = clock();
        printf("Processing Time: %lf\n", (double)(end-start)/CLOCKS_PER_SEC);
        printf("The Number of Process: %d\n", cnt_p+1);
    }
}
