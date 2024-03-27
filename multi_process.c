#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main() {
    // 프로세스 생성 개수
    int N = 8;
    
    // 1000을 N등분
    int num = 1000/N;

    // 0*125+(1~125) / 1*125+(1~125) / 2*125+(1~125) ... cnt*125+(1~125)
    int cnt = -1;

    // 가장 먼저 생성된 프로세스인지 아닌지 확인하는 값
    int not_root = 0;
                                                
    int status;
    pid_t pid;

    clock_t start = clock();    // 프로그램 수행 시간 측정
    for(int i = 0; i < N; i++) {
        // 프로세스 생성
        pid = fork();
        cnt++;
        
        if(pid == 0) {
            printf("%d번째 프로세스 생성\n", cnt+1);
            not_root = 1;
            break;
        }
    }

    if(not_root) {
        for(int i = 1; i < num+1; i++) {
            printf("%d * 7 = %d\n", cnt*num+i, (cnt*num+i)*7);
        }
    }
    else {
        while(wait(&status) != pid) continue; // 자식 프로세스가 끝날 때 까지 대기
        clock_t end = clock();
        printf("Processing Time: %lf\n", (double)(end-start));   
    }
}
