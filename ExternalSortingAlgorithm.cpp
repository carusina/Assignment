#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <numeric>

#define MAX 1000
#define MEMORY_SIZE 100

using namespace std;

void InsertionSort(vector<int>& nums) {
    for(size_t i = 1; i <= nums.size()-1; i++) {
        int CurrentElement = nums[i];
        int j = i-1;
        while(j >= 0 && nums[j] > CurrentElement) {
            nums[j+1] = nums[j];
            j--;
        }
        nums[j+1] = CurrentElement;
    }
}

void MergeBlocks(vector<int>& memory, int input_HDD[], int block1_index, int block2_index, int block1_size, int block2_size, int output_HDD[]) {
    int i = block1_index, j = block2_index;
    int k = block1_index;
    int end1 = block1_index + block1_size;
    int end2 = block2_index + block2_size;
    
    while(i < end1 && j < end2) {
        memory.clear();

        while(memory.size() < MEMORY_SIZE && i < end1 && j < end2) {
            if(input_HDD[i] < input_HDD[j]) {
                memory.push_back(input_HDD[i++]);
            }
            else {
                memory.push_back(input_HDD[j++]);
            }
        }

        for(int num : memory) {
            output_HDD[k++] = num;
        }
    }

    while(i < end1) {
        memory.clear();

        while(memory.size() < MEMORY_SIZE && i < end1) {
            memory.push_back(input_HDD[i++]);
        }
        
        for(int num : memory) {
            output_HDD[k++] = num;
        }
    }
    while(j < end2) {
        memory.clear();
        
        while(memory.size() < MEMORY_SIZE && j < end2) {
            memory.push_back(input_HDD[j++]);
        }
        
        for(int num : memory) {
            output_HDD[k++] = num;
        }
    }
}

void ExternalSort(int input_HDD[], int n, int output_HDD[]) {
    vector<int> memory;
    vector<int> BlockSizes;

    for(int i = 0; i < n; i += MEMORY_SIZE) {
        memory.clear();
        
        for(int j = i; j < i+MEMORY_SIZE && j < n; j++) {
            memory.push_back(input_HDD[j]);
        }
        InsertionSort(memory);
        copy(memory.begin(), memory.end(), input_HDD+i);
        BlockSizes.push_back(memory.size());
    }

    while(BlockSizes.size() > 1) {
        vector<int> newBlockSizes;

        for(int i = 0; i < BlockSizes.size(); i += 2) {
            if(i+1 < BlockSizes.size()) {
                int block1_index = accumulate(BlockSizes.begin(), BlockSizes.begin()+i, 0);
                int block2_index = block1_index + BlockSizes[i];

                MergeBlocks(memory, input_HDD, block1_index, block2_index, BlockSizes[i], BlockSizes[i+1], output_HDD);
                newBlockSizes.push_back(BlockSizes[i]+BlockSizes[i+1]);
            }
            else {
                int block1_index = accumulate(BlockSizes.begin(), BlockSizes.begin()+i, 0);
                int k = block1_index;
                for(int j = 0; j < BlockSizes[i]; j++) {
                    output_HDD[k++] = output_HDD[block1_index + j];
                }
                newBlockSizes.push_back(BlockSizes[i]);
            }
        }
        int* temp = input_HDD;
        input_HDD = output_HDD;
        output_HDD = temp;
        
        BlockSizes = newBlockSizes;
    }
}

int main() {
    ifstream inputFile("input.txt");
    if(!inputFile.is_open()) {
        cerr << "파일을 열 수 없음\n";
        return 1;
    }

    int n = 0;
    int input_nums[MAX];
    int output_nums[MAX];
    string line;
    while(getline(inputFile, line)) {
        input_nums[n++] = stoi(line);
    }
    inputFile.close();

    ExternalSort(input_nums, n, output_nums);

    ofstream outputFile("external_output.txt");
    if(!outputFile.is_open()) {
        cerr << "파일을 열 수 없음\n";
        return 1;
    }

    for(int num : input_nums) {
        outputFile << num << endl;
    }
    outputFile.close();

    return 0;
}