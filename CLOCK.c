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
    int bit;
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
        newp->bit = 0;

        if(head == NULL) head = newp;
        else tail->next = newp;

        tail = newp;
        tail->next = head;
    }

    for(int i = 0; i < NUM; i++) {
        int is_page = 0;

        struct List* cur = head;
        for(int j = 0; j < FRAMENUM; j++) {
            if(cur->page == ref[i]) {
                is_page = 1;
                cur->bit = 1;
                break;
            }
            cur = cur->next;
        }

        if(is_page == 1) {
            cur->next->bit = 0;
            cur->next->next->bit = 0;
            for(int j = 0; j < FRAMENUM; j++) frame[i+1][j] = frame[i][j];
        }
        else {
            if(head->bit != 0) {
                while(head->bit != 0) {
                    head->bit = 0;
                    head = head->next;
                }
            }

            int index = 0;
            for(int j = 0; j < FRAMENUM; j++) {
                if(frame[i][j] == head->page) {
                    frame[i+1][j] = ref[i];
                    index = j;
                    break;
                }
            }
            for(int j = 0; j < FRAMENUM; j++) {
                if(j != index) frame[i+1][j] = frame[i][j];
            }

            head->bit = 1;
            head->page = ref[i];
            head = head->next;

            fault[i] = 1;
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
