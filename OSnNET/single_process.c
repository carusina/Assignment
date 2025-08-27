#include <stdio.h>
#include <time.h>

int main() {
    int num = 1000;
    
    clock_t start = clock();
    for(int i = 1; i < num+1; i++) {
        printf("%d * 7 = %d\n", i, i*7);
    }
    clock_t end = clock();

    printf("Processing Time: %lf\n", (double)(end-start)/CLOCKS_PER_SEC);
}
