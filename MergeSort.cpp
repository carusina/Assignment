#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

ifstream fin("input_sort.txt");
ofstream fout("output_merge_sort.xlsx");

int temp[100];

void MergeSort(int A[], int p, int q) {
    if(p < q) {
        int k = (p+q)/2;
        MergeSort(A, p, k);
        MergeSort(A, k+1, q);

        int i = p, j = k+1, t = 0;
        while(i <= k && j <= q) {
            if(A[i] <= A[j]) temp[t++] = A[i++];
            else temp[t++] = A[j++];
        }
        while(i <= k) temp[t++] = A[i++];
        while(j <= q) temp[t++] = A[j++];
        
        i = p; t = 0;
        while(i <= q) A[i++] = temp[t++];
    }
}

int main() {
    int input[100];

    int cnt = 0;
    while(fin >> input[cnt]) {  cnt++;  } 

    clock_t strat = clock();
    MergeSort(input, 0, cnt-1);
    clock_t finish = clock();

    for(; cnt > 0; cnt--) {
        fout << input[100-cnt] << endl;
    }
    printf("running time (ms): %f\n", (double)(finish-strat));

    fin.close();
    fout.close();
}