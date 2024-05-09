#include <stdio.h>
#include<stdlib.h>
#include <pthread.h>

#define ProcessNum 5

int current_time = 0;
int is_finish = 0;
int is_timed = 1;

int arrive_time[ProcessNum] = {0,1,2,3,4};
int need_time[ProcessNum] = {10,28,6,4,14};

int running_time[ProcessNum];
int wait_time[ProcessNum];
int start_time[ProcessNum];
int end_time[ProcessNum];

struct List {
    struct List* next;
    int num;
    int priority;
};
struct List* head = NULL;
struct List* tail = NULL;


// struct Args {
//     struct List* process;
//     int num;
// };

void scheduling(struct List* process) {
    if(head == NULL) {
        head = process;
        return;
    }
    // else tail->next = process;

    struct List* cur = head->next;
    struct List* prev = head;
    while(process->priority > cur->priority) {
        prev = cur;
        cur = cur->next;
    }
    prev->next = process;
    process->next = cur;
}

void* func(void* args) {
    struct List* input = (struct List*)args;

    scheduling(input);

    while(running_time[input->num] < need_time[input->num]) {
        if(input == head) {
            if(start_time[input->num] == 0) start_time[input->num] = current_time;

            printf("P%d: %d X %d = %d\n", input->num+1, running_time[input->num]+1, input->num+1, (running_time[input->num]+1)*(input->num+1));
            running_time[input->num]++;
            current_time++;
            is_timed = 1;
        }
        while(is_finish == 0 && is_timed == 1) {}
    }
    end_time[input->num] = current_time;
    head = input->next;
}

int main() {
    pthread_t tid[ProcessNum];
    int num = 0;

    while(current_time <= arrive_time[ProcessNum-1]) {
        if(current_time == arrive_time[num]) {
            struct List* newp = (struct List*)malloc(sizeof(struct List));
            newp->priority = current_time;
            newp->next = NULL;
            newp->num = num;
            
            pthread_create(&tid[num], NULL, func, (void*)newp);
            num++;
        }
        is_timed = 0;
    }
    is_finish = 1;
    for(int i = 0; i < num; i++) pthread_join(tid[i], NULL);
}
