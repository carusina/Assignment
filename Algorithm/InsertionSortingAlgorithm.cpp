#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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

    InsertionSort(input_nums);

    ofstream outputFile("insertion_output.txt");
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