#include <stdio.h>
#include <time.h>

int mid(int inputs[], int s, int e) {
    int mid = (s+e)/2;

    if(inputs[s] >= inputs[mid]) {
        if(inputs[s] <= inputs[e]) return s;
        else if(inputs[mid] >= inputs[e]) return mid;
        else return e;
    }
    else {
        if(inputs[s] >= inputs[e]) return s;
        else if(inputs[mid] <= inputs[e]) return mid;
        else return e;
    }
}

int pivot(int inputs[], int s, int e) {
    int div = (e-s+1)/3;  if(div < 1) return s;
    if(e-s+1 == div*3+1) div -=1;

    int first = mid(inputs, s, s+div);
    int second = mid(inputs, s+div+1, s+2*div+1);
    int third = mid(inputs, s+2*div+2, e);

    if(inputs[first] >= inputs[second]) {
        if(inputs[first] <= inputs[third]) return first;
        else if(inputs[second] >= inputs[third]) return second;
        else return third;
    }
    else {
        if(inputs[first] >= inputs[third]) return first;
        else if(inputs[second] <= inputs[third]) return second;
        else return third;
    }
}

int Selection(int inputs[], int s, int e, int k) {
    if(s <= e) {
        int p = pivot(inputs, s, e);
        int temp = inputs[s];   inputs[s] = inputs[p];  inputs[p] = temp;

        p = s;  int p_e = e;
        for(int i = s+1; i <= p_e; i++, p++) {
            if(inputs[s] < inputs[i]) {
                for(; p_e > p; p_e--) {
                    if(inputs[p_e] <= inputs[s]) {
                        temp = inputs[p_e]; inputs[p_e] = inputs[i]; inputs[i] = temp;
                        break;
                    }
                }
                if(p_e <= p) break;
            }
        }
        temp = inputs[s]; inputs[s] = inputs[p]; inputs[p] = temp;

        int S = (p-1)-s+1;
        if(k <= S)  return Selection(inputs, s, p-1, k);
        else if(k == S+1) return inputs[p];
        else return Selection(inputs, p+1, e, k-S-1);
    }
    
    return -99999;
}

int main() {
    int inputs[100], cnt = 0;
    FILE *fp = fopen("input_sort.txt", "r");

    while(fscanf(fp, "%d", &inputs[cnt]) == 1) {    cnt++;  }
    fclose(fp);

    clock_t start = clock();
    printf("50번째: %d\n70번째: %d\n", Selection(inputs, 0, cnt-1, 50), Selection(inputs, 0, cnt-1, 70));
    clock_t finish = clock();
    printf("running time (ms): %lu\n", finish-start);
}
