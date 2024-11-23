#include <stdio.h>
#include <stdlib.h>

#define MAX 100

void SelectionSort(int nums[], int n) {
    for(int i = 0; i <= n-1; i++) {
        int min = i;

        for(int j = i+1; j <= n; j++) {
            if(nums[j] < nums[min]) {
                min = j;
            }
        }
        int temp = nums[min];
        nums[min] = nums[i];
        nums[i] = temp;
    }
}

int main() {
    FILE *inputFile = fopen("input.txt", "r");
    if(inputFile == NULL) {
        printf("파일을 열 수 없음\n");
        return 1;
    }

    int *input_nums = (int*)malloc(MAX * sizeof(int));
    if(input_nums == NULL) {
        printf("메모리 할당 실패\n");
        return 1;
    }

    int n = 0, capacity = MAX;
    while(fscanf(inputFile, "%d", &input_nums[n]) == 1) {
        n++;
        if(n >= capacity) {
            capacity *= 2;
            input_nums = (int*)realloc(input_nums, capacity * sizeof(int));
            if(input_nums == NULL) {
                printf("메모리 할당 실패\n");
                return 1;
            }
        }
    }
    fclose(inputFile);

    SelectionSort(input_nums, n-1);

    FILE *outputFile = fopen("selection_output.txt", "w");
    if(outputFile == NULL) {
        printf("파일을 열 수 없음\n");
        return 1;
    }

    for(int i = 0; i < n; i++) {
        fprintf(outputFile, "%d\n", input_nums[i]);
    }
    fclose(outputFile);
    free(input_nums);

    return  0;
}