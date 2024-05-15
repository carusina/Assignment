#include <stdio.h>
#include<stdlib.h>
#include <pthread.h>

#define ProcessNum 5

int current_time = 0;
int is_finish = 0;
int is_timed = 1;
int is_scheduling = 0;

int arrive_time[ProcessNum] = {0,1,2,3,4};
int need_time[ProcessNum] = {10,28,6,4,14};

int running_time[ProcessNum];
int wait_time[ProcessNum];
int start_time[ProcessNum];
int end_time[ProcessNum];

int finish_order[ProcessNum];
int order = 0;

struct List {
    struct List* next;
    int num;
    int priority;
};
struct List* head = NULL;
struct List* tail = NULL;

void scheduling(struct List* process) {
    while(is_scheduling != process->num) {}

    struct List* prev = head;
    struct List* cur = head->next;

    while(cur != NULL && process->priority > cur->priority) {
        prev = cur;
        cur = cur->next;
    }
    prev->next = process;
    process->next = cur;

    is_scheduling++;
}

void* func(void* args) {
    struct List* input = (struct List*)args;

    while(running_time[input->num] < need_time[input->num]) {
        while(is_timed == 1 && is_finish == 0) {}
        if(input == head) {
            if(start_time[input->num] == -1) start_time[input->num] = current_time;

            printf("P%d: %d X %d = %d\n", input->num+1, running_time[input->num]+1, input->num+1, (running_time[input->num]+1)*(input->num+1));
            running_time[input->num]++;
            current_time++;
            is_timed = 1;
        }
    }
    end_time[input->num] = current_time;
    head = input->next;
    finish_order[order++] = input->num;

    free(input);
    pthread_exit(NULL);
}

int main() {
    for(int i = 0; i < ProcessNum; i++) start_time[i] = -1;

    pthread_t tid[ProcessNum];
    int num = 0;

    while(current_time <= arrive_time[ProcessNum-1]) {
        if(current_time == arrive_time[num] && is_timed == 1) {
            struct List* newp = (struct List*)malloc(sizeof(struct List));
            newp->priority = need_time[num];
            newp->next = NULL;
            newp->num = num;
            if(num == 0) head = newp;
            
            scheduling(newp);
            pthread_create(&tid[num], NULL, func, (void*)newp);
            
            num++;
            is_timed = 0;
        }
    }
    is_finish = 1;
    for(int i = 0; i < num; i++) pthread_join(tid[i], NULL);
    for(int i = 0; i < num; i++) printf("P%d (%d-%d)\n", finish_order[i]+1, start_time[finish_order[i]], end_time[finish_order[i]]);

    int sum_return_time = 0;
    int sum_waiting_time = 0;
    for(int i = 0; i < ProcessNum; i++) {
        int return_time = end_time[i]-arrive_time[i];
        int waiting_time = end_time[i]-running_time[i]-arrive_time[i];
        printf("P%d 반환시간: %2d 대기시간: %2d\n", i+1, return_time, waiting_time);
        sum_return_time += return_time;
        sum_waiting_time += waiting_time;
    }
    printf("평균 반환시간: %lf 평균 대기시간: %lf\n", (double)sum_return_time/ProcessNum, (double)sum_waiting_time/ProcessNum);
}
