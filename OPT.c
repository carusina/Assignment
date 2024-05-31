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
    int seq;
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
        newp->seq = 0;

        if(head == NULL) head = newp;
        else tail->next = newp;

        tail = newp;
    }

    for(int i = 0; i < NUM; i++) {
        int is_page = 0;
        int min = NUM+1;
        for(int j = i+1; j < NUM; j++) {
            if(ref[i] == ref[j]) {
                min = j;
                break;
            }
        }
        
        struct List* temp = head;
        for(int j = 0; j < FRAMENUM; j++) {
            if(temp->page == ref[i]) {
                is_page = 1;
                temp->seq = min;
                
                struct List* prev = NULL;
                struct List* cur = head;
                while(cur != NULL && temp->seq <= cur->seq) {
                    prev = cur;
                    cur = cur->next;
                }
                if(prev != temp) {
                    if(prev == NULL) {
                        temp->next = head;
                        head = temp;

                        cur = head;
                        while(cur != NULL && cur->next != temp) cur = cur->next;
                        cur->next = NULL;
                    }
                    else {
                        cur->next = temp->next;
                        temp->next = cur;
                        prev->next = temp;
                    }
                }

                break;
            }
            temp = temp->next;
        }
        if(is_page == 0) {
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
            
            head->page = ref[i];
            head->seq = min;

            struct List* prev = head;
            struct List* cur = head->next;
            while(cur != NULL && (head->seq <= cur->seq || cur->seq == 0)) {
                prev = cur;
                cur = cur->next;
            }
            
            struct List* temp = head->next;
            if(prev != head) {
                head->next = prev->next;
                prev->next = head;
                head = temp;
            }

            fault[i] = 1;
        }
        else for(int j = 0; j < FRAMENUM; j++) frame[i+1][j] = frame[i][j];
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
