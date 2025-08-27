#include <stdio.h>
#define Capacity 10

int Check_Capacity(int B, int Bins[][Capacity], int item, int method) {
    if(B == 0) {
        return -1;
    }
    // First Fit
    if(method == 1) {
        for(int i = 0; i < B; i++) {
            int Bin_sum = 0;

            for(int j = 0; j < Capacity; j++) {
                if(Bins[i][j] == 0) {
                    break;
                }

                Bin_sum += Bins[i][j];
            }
            if(Bin_sum + item <= Capacity) {
                return i;
            }
        }
    }
    // Next Fit
    else if(method == 2) {
        int Bin_sum = 0;

        for(int j = 0; j < Capacity; j++) {
            if(Bins[B-1][j] == 0) {
                break;
            }

            Bin_sum += Bins[B-1][j];
        }
        if(Bin_sum + item <= Capacity) {
            return B-1;
        }
    }
    // Best Fit
    else if(method == 3) {
        int max = 0;
        int max_idx = -1;

        for(int i = 0; i < B; i++) {
            int Bin_sum = 0;

            for(int j = 0; j < Capacity; j++) {
                if(Bins[i][j] == 0) {
                    break;
                }
                Bin_sum += Bins[i][j];
            }
            if(Bin_sum + item <= Capacity && max < Bin_sum) {
                max = Bin_sum;
                max_idx = i;
            }
        }
        if(max_idx != -1) {
            return max_idx;
        }
    }
    // Worst Fit
    else {
        int min = Capacity;
        int min_idx = -1;

        for(int i = 0; i < B; i++) {
            int Bin_sum = 0;

            for(int j = 0; j < Capacity; j++) {
                if(Bins[i][j] == 0) {
                    break;
                }
                Bin_sum += Bins[i][j];
            }
            if(Bin_sum + item <= Capacity && min > Bin_sum) {
                min = Bin_sum;
                min_idx = i;
            }
        }
        if(min_idx != -1) {
            return min_idx;
        }
    }
    
    return -1;
}

// method = 1: First Fit, 2: Next Fit, 3: Best Fit, 4: Worst Fit
int Approx_BinPacking(int Bins[][Capacity], int items[], int n, int method) {
    int B = 0;

    for(int i = 0; i < n; i++) {
        int idx = Check_Capacity(B, Bins, items[i], method);
        if(idx != -1) {
            for(int j = 0; j < Capacity; j++) {
                if(Bins[idx][j] == 0) {
                    Bins[idx][j] = items[i];
                    break;
                }
            }
        }
        else {
            Bins[B++][0] = items[i];
        }
    }

    return B;
}
void print_BinPacking(int Bins[][Capacity], int B, int method) {
    if(method == 1) {
        printf("Output1 (First Fit): ");
    } else if(method == 2) {
        printf("Output2 (Next Fit): ");
    } else if(method == 3) {
        printf("Output3 (Best Fit): ");
    } else {
        printf("Output4 (Worst Fit): ");
    }
    
    for(int i = 0; i < B; i++) {
        printf("bin%d = [", i+1);
        for(int j = 0; j < Capacity; j++) {
            if(j == Capacity-1 || Bins[i][j+1] == 0) {
                printf("%d ", Bins[i][j]);
                break;
            }
            printf("%d, ", Bins[i][j]);
        }
        printf("], ");
    }
    printf("\n");
}

int main() {
    int items[] = {7, 5, 6, 4, 2, 3, 7, 5};
    
    for(int method = 1; method < 5; method++) {
        int Bins[Capacity][Capacity] = {0};
        int B = Approx_BinPacking(Bins, items, 8, method);
        print_BinPacking(Bins, B, method);
    }
    
    return 0;
}