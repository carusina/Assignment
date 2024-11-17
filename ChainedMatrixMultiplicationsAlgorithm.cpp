#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>

#define INFINITY std::numeric_limits<int>::max()

using namespace std;

#define MAX 100

int getInput(ifstream& input, int dimensions[][2]) {
    int n = 0, rows = 0, cols = 0;
    string line;

    while(getline(input, line)) {
        if(line.find("[") != string::npos) rows++;
        if(rows == 1) {
            size_t pos = line.find("[[");
            line = (pos != string::npos) ? line.substr(pos+1) : line;
            stringstream ss(line);
            string temp;

            while(ss >> temp) {
                if(temp != "[" && temp != "]") {
                    cols++;
                }
            }
        }

        if(line.find("]]") != string::npos) {
            dimensions[n][0] = rows;
            dimensions[n][1] = cols;
            n++;

            rows = 0; cols = 0;
        }
    }

    return n;
}

int ChainedMatirxMultiplications(int dimensions[][2], int C[][MAX], int n) {
    for(int i = 0; i <= n; i++) {
        C[i][i] = 0;
    }
    for(int L = 1; L <= n-1; L++) {
        for(int i = 0; i <= n-L; i++) {
            int j = i+L;
            C[i][j] = INFINITY;
            for(int k = i; k <= j-1; k++) {
                int temp = C[i][k]+C[k+1][j]+dimensions[i][0]*dimensions[k][1]*dimensions[j][1];
                if(temp < C[i][j]) C[i][j] = temp;
            }
        }
    }
    return C[0][n-1];
}

void printC(int C[][MAX], int n) {
    printf("C      ");
    for (int i = 1; i < n+1; i++) printf("%-10d", i);
    printf("\n");

    int k = 1;
    for (int i = 1; i < n+1; i++, k++) {
        printf("%-7d", i);
        for (int j = 1; j < n+1; j++) {
            if(j < k) printf("          ");
            else printf("%-10d", C[i-1][j-1]);
        }
        printf("\n");
    }
}

int main() {
    ifstream input("matrix_input.txt");
    if(!input.is_open()) {
        cerr << "파일을 열 수 없음.\n";
        return 1;
    }

    int dimensions[MAX][2] = {0};
    int n = getInput(input, dimensions);

    int C[MAX][MAX];
    int min = ChainedMatirxMultiplications(dimensions, C, n);

    printC(C, n);
    printf("Minimum number of multiplications: %d\n", min);

    return 0;
}
