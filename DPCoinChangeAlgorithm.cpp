#include <iostream>
#include <limits>

#define INFINITY std::numeric_limits<int>::max()

using namespace std;

int DPCoinChange(int n, int d[], int k, int C[]) {
    for(int i = 1; i <= n; i++) C[i] = INFINITY;
    C[0] = 0;

    for(int j = 1; j <= n; j++) {
        for(int i = 0; i < k; i++) {
            if(d[i] <= j && C[j-d[i]]+1 <C[j]) C[j] = C[j-d[i]]+1;
        } 
    } 
    return C[n];
}

void printCoin(int n, int C[]) {
    printf("j ");
    for(int i = 0; i <= n; i++) printf("%2d  ", i);
    printf("\nc ");
    for(int i = 0; i <= n; i++) printf("%2d  ", C[i]);
    printf("\n");
}

int main() {
    int n = 20;
    int d[4] = {16, 10, 5, 1};
    int C[n+1];
    DPCoinChange(n, d, 4, C);
    printCoin(n, C);
}