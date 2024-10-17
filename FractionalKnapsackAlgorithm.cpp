// must needed cmd option '-std=c++11'

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <ctime>

using namespace std;

#define MAX_WEIGHT 40

struct Goods {
    string name;
    double weight;
    double value;

    double ValuePerWeight() const {
        return value/weight;
    }
};

struct Knapsack {
    vector<Goods> goods;
    int max_weight;

    double getKnapsackWeight() const {
        double weight = 0;
        if(!goods.empty()) {
            for(const auto& item : goods) {
                weight += item.weight;
            }
        }
        return weight;
    }

    double getKnapsackValue() const {
        double value = 0;
        if(!goods.empty()) {
            for(const auto& item : goods) {
                value += item.value;
            }
        }
        return value;
    }
};

vector<Goods> readInputFile(const string& filename) {
    ifstream input(filename);

    if (!input.is_open()) {
        cerr << "파일을 열 수 없음\n";
        return {};
    }

    vector<Goods> goods;
    string line;

    while (getline(input, line)) {
        line.erase(remove(line.begin(), line.end(), '('), line.end());
        line.erase(remove(line.begin(), line.end(), ')'), line.end());

        stringstream ss(line);
        string name;
        double weight, value;
        char comma;

        getline(ss, name, ','); // 이름은 공백 포함 가능하므로 getline 사용
        ss >> weight >> comma >> value;

        goods.push_back({name, weight, value});
    }
    input.close();

    return goods;
}


Knapsack FractionalKnapsack(vector<Goods>& goods) {
    Knapsack knapsack;
    knapsack.max_weight = MAX_WEIGHT;

    sort(goods.begin(), goods.end(), [](const Goods& a, const Goods& b) {
        return a.ValuePerWeight() > b.ValuePerWeight();
    });

    while(!goods.empty() && knapsack.getKnapsackWeight() + goods.front().weight <= knapsack.max_weight) {
        knapsack.goods.push_back(goods.front());
        goods.erase(goods.begin());
    }

    double space = knapsack.max_weight - knapsack.getKnapsackWeight();
    if(!goods.empty() && space > 0) {
        knapsack.goods.push_back({goods.front().name, space, space * goods.front().ValuePerWeight()});
        
        goods.front().value -= (space * goods.front().ValuePerWeight());
        goods.front().weight -= space;
    }

    return knapsack;
}

void printKnapsack(Knapsack knapsack) {
    printf("Goods\tWeight of goods in knapsack\tValue of goods in kanpsack\n");
    for(const auto& item : knapsack.goods) {
        printf("%-5s\t\t%-10f\t\t\t%-10f\n", item.name.c_str(), item.weight, item.value);
    }
    printf("Sum  \t\t%-10f\t\t\t%-10f\n", knapsack.getKnapsackWeight(), knapsack.getKnapsackValue());
}

int main() {
    vector<Goods> goods = readInputFile("input_Fractional.txt");
    clock_t start = clock();
    Knapsack knapsack = FractionalKnapsack(goods);
    clock_t finish = clock();

    printKnapsack(knapsack);
    printf("running time (ms): %f\n", (double)(finish-start));
    
    return 0;
}