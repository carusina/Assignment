#include <stdio.h>
#include <time.h>

int main() {
    int num = 100;
    int muls[] = {3, 5, 7, 9};
    
    clock_t start = clock();
    for(int i = 0; i < 4; i++) {
        for(int j = 1; j < num+1; j++) {
            printf("%d * %d = %d\n", j, muls[i], j*muls[i]);
        }
    }
    clock_t end = clock();

    printf("Processing Time: %lf\n", (double)(end-start)/CLOCKS_PER_SEC);
}
