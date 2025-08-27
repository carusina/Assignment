#include <stdio.h>
#include <stdlib.h>

#define NUM 20
#define FRAMENUM 3
int frame[NUM+1][FRAMENUM];
int ref[NUM] = {1,2,3,2,1,5,2,1,6,2,5,6,3,1,3,6,1,2,4,3};
int fault[NUM];

struct List {
    struct List* next;
    int page;
};
struct List* head = NULL;
struct List* tail = NULL;

int main() {
    for(int i = 0; i < FRAMENUM; i++) fault[i] = 0;

    for(int i = 0; i < FRAMENUM; i++) {
        frame[0][i] = 0;

        struct List* newp = (struct List*)malloc(sizeof(struct List));
        newp->next = NULL;
        newp->page = frame[0][i];

        if(head == NULL) head = newp;
        else tail->next = newp;

        tail = newp;
    }

    for(int i = 0; i < NUM; i++) {
        int is_page = 0;

        struct List* prev = NULL;
        struct List* cur = head;
        for(int j = 0; j < FRAMENUM; j++) {
            if(cur->page == ref[i]) {
                if(prev != NULL) {
                    prev->next = cur->next;
                    cur->next = head;
                    head = cur;
                    if(cur == tail) tail = prev;
                }
                is_page = 1;
                break;
            }
            prev = cur;
            cur = cur->next;
        }
       
        if(is_page == 0) {
            prev = NULL;
            cur = head;
            while(cur != tail) {
                prev = cur;
                cur = cur->next;
            }
            cur->page = ref[i];
            prev->next = cur->next;
            cur->next = head;
            head = cur;
            tail = prev;

            fault[i] = 1;
        }
        cur = head;
        for(int j = 0; j < FRAMENUM; j++) {
            frame[i+1][j] = cur->page;
            cur = cur->next;
        }
    }

    for(int i = 0; i < NUM+1; i++) {
        printf("(T: %2d) = ", i);
        for(int j = 0; j < FRAMENUM; j++) {
            if(j == 0) printf("[ ");
            printf("%d ", frame[i][j]);
            if(j == FRAMENUM-1) printf("]\n");
        }
    }
     for(int i = 0; i < NUM; i++) {
        if(i == 0) printf("Page Fault: ( ");
        if(fault[i] == 1) printf("%d ", i);
        if(i == NUM-1) printf(")\n");
    }
}
