#include <stdio.h>
#include <stdlib.h>

#define MAX 100

void DownHeap(int nums[], int n, int i) {
    int leftChild = 2*i;
    int rightChild = 2*i+1;
    int bigger;

    if((leftChild <= n) && (nums[leftChild] > nums[i])) {
        bigger = leftChild;
    }
    else {
        bigger = i;
    }
    if((rightChild <= n) && (nums[rightChild] > nums[bigger])) {
        bigger = rightChild;
    }
    if(bigger != i) {
        int temp = nums[i];
        nums[i] = nums[bigger];
        nums[bigger] = temp;
        
        DownHeap(nums, n, bigger);
    }
}

void BuildHeap(int nums[], int n) {
    for(int i = n/2; i >= 0; i--) {
        DownHeap(nums, n, i);
    }
}

void HeapSort(int nums[], int n) {
    BuildHeap(nums, n);

    int heapSize = n;
    for(int i = 0; i <= n-1; i++) {
        int temp = nums[0];
        nums[0] = nums[heapSize];
        nums[heapSize] = temp;
        heapSize--;

        DownHeap(nums, heapSize, 0);
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

    HeapSort(input_nums, n-1);

    FILE *outputFile = fopen("heap_output.txt", "w");
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