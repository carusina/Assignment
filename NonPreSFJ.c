#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define ProcessNum 5

int current_time = 0;
int is_finish = 0;
int is_timed = 1;
int is_scheduling = 0;
int is_in = 0;

int arrive_time[ProcessNum] = {0,1,2,3,4};
int need_time[ProcessNum] = {10,28,6,4,14};

int running_time[ProcessNum];
int drop_time[ProcessNum];
int start_time[ProcessNum];
int end_time[ProcessNum];

int finish_order[ProcessNum];
int order = 0;
char buf[999];
char gant[999];

struct List {
    struct List* next;
    int num;
    int priority;
};
struct List* head = NULL;
struct List* tail = NULL;

void scheduling(struct List* process, int is_first) {
    if(is_first == 1) {
        while(is_scheduling != process->num) {}

        if(head == NULL) {
            head = process;
            is_scheduling++;
            is_in = 0;
            return;
        }

        struct List* prev = NULL;
        struct List* cur = head;

        while(cur != NULL && process->priority > cur->priority) {
            prev = cur;
            cur = cur->next;
        }
        if(prev == NULL) {
            sprintf(buf, "P%d (%d-%d)\n", head->num+1, start_time[head->num], current_time);
            strcat(gant, buf);
            start_time[head->num] = -1;

            process->next = head;
            head = process;
        }
        else {
            prev->next = process;
            process->next = cur;
        }
        
        is_scheduling++;
        is_in = 0;
    }
    else {
        while(is_in == 1) {}

        struct List* temp;
        while(process->next != NULL && process->priority > process->next->priority) {
            temp = process->next;
            process->next = process->next->next;
            temp->next = process;
            head = temp;
        }
        if(process != head) {
            sprintf(buf, "P%d (%d-%d)\n", head->num+1, start_time[head->num], current_time);
            strcat(gant, buf);
            start_time[head->num] = -1;
        }
    }
}

void* func(void* args) {
    struct List* input = (struct List*)args;

    while(running_time[input->num] < need_time[input->num]) {
        int cnt = 0;

        while(is_finish == 0 && is_timed == 1) {}
        if(input == head) {
            if(start_time[input->num] == -1) start_time[input->num] = current_time;
            printf("P%d: %d X %d = %d\n", input->num+1, running_time[input->num]+1, input->num+1, (running_time[input->num]+1)*(input->num+1));
            running_time[input->num]++;
            input->priority = need_time[input->num];
            // input->priority = need_time[input->num] - running_time[input->num];
            scheduling(input, 0);
            current_time++;
            cnt++;
            is_timed = 1;
        }
    }
    end_time[input->num] = current_time;
    head = input->next;
    finish_order[order++] = input->num;
    sprintf(buf, "P%d (%d-%d)\n", input->num+1, start_time[input->num], end_time[input->num]);
    strcat(gant, buf);

    free(input);
    pthread_exit(NULL);
}

int main() {
    for(int i = 0; i < ProcessNum; i++) start_time[i] = -1;
    for(int i = 0; i < ProcessNum; i++) drop_time[i] = -1;

    pthread_t tid[ProcessNum];
    int num = 0;

    while(current_time <= arrive_time[ProcessNum-1]) {
        if(current_time == arrive_time[num] && is_timed == 1) {
            struct List* newp = (struct List*)malloc(sizeof(struct List));
            newp->priority = need_time[num] - running_time[num];
            newp->next = NULL;
            newp->num = num;
            
            is_in = 1;
            scheduling(newp, 1);
            pthread_create(&tid[num], NULL, func, (void*)newp);
            
            num++;
            is_timed = 0;
        }
    }
    is_finish = 1;
    for(int i = 0; i < num; i++) pthread_join(tid[i], NULL);
    printf("%s",gant);

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
