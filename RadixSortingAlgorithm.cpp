#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

int getDigit(int num, int digitPosition) {
    return (num / static_cast<int>(pow(10, digitPosition-1))) % 10;
}

void RadixSortLSD(vector<int>& nums) {
    int max = *max_element(nums.begin(), nums.end());
    int k = 0;
    while(max > 0) {
        max /= 10;
        k++;
    }

    for(int i = 1; i <= k; i++) {
        vector<vector<int> > temps(10);
        
        for(int num : nums) {
            temps[getDigit(num, i)].push_back(num);
        }

        nums.clear();
        for(const auto& temp: temps) {
            for(int num : temp) {
                nums.push_back(num);
            }
        }
    }
}

void RadixSortMSDHelper(vector<int>& nums, int k) {
    if(k <= 0) return;

    vector<vector<int> > temps(10);

    for(int num : nums) {
        temps[getDigit(num, k)].push_back(num);
    }

    nums.clear();
    for(auto& temp: temps) {
        RadixSortMSDHelper(temp, k-1);

        for(int num : temp) {
            nums.push_back(num);
        }
    }
}

void RadixSortMSD(vector<int>& nums) {
    int max = *max_element(nums.begin(), nums.end());
    int k = 0;
    while(max > 0) {
        max /= 10;
        k++;
    }

    RadixSortMSDHelper(nums, k);
}

int main() {
    ifstream inputFile("input.txt");
    if(!inputFile.is_open()) {
        cerr << "파일을 열 수 없음\n";
        return 1;
    }

    vector<int> input_nums;
    string line;
    while(getline(inputFile, line)) {
        input_nums.push_back(stoi(line));
    }
    inputFile.close();

    vector<int> input_nums_LSD = input_nums;
    RadixSortLSD(input_nums_LSD);

    ofstream outputFile1("radix_lsd_output.txt");
    if(!outputFile1.is_open()) {
        cerr << "파일을 열 수 없음\n";
        return 1;
    }

    for(int num : input_nums_LSD) {
        outputFile1 << num << endl;
    }
    outputFile1.close();

    vector<int> input_nums_MSD = input_nums;
    RadixSortMSD(input_nums_MSD);

    ofstream outputFile2("radix_msd_output.txt");
    if(!outputFile2.is_open()) {
        cerr << "파일을 열 수 없음\n";
        return 1;
    }

    for(int num : input_nums_MSD) {
        outputFile2 << num << endl;
    }
    outputFile2.close();

    return 0;
}