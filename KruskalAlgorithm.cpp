// must nedded cmd option '-std=c++11'

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <sstream>
#include <set>
#include <ctime>

using namespace std;

bool is_Cycle(vector<tuple<char, char, int> > tree, char start, char end) {
    for(const auto& edge : tree) {
        if(get<0>(edge) == start && get<1>(edge) == end) return 1;
        else if(get<0>(edge) == start && get<1>(edge) != end) return is_Cycle(tree, get<1>(edge), end);
    }
    return 0;
}

vector<tuple<char, char, int> > KrusalMST(vector<tuple<char, char, int> > edges, set<char> verteices) {
    sort(edges.begin(), edges.end(), [](const tuple<char, char, int>& a, const tuple<char, char, int>& b){
        if (get<2>(a) != get<2>(b)) return get<2>(a) < get<2>(b);
        else if (get<0>(a) != get<0>(b))    return get<0>(a) < get<0>(b);
        else    return get<1>(a) < get<1>(b);
    });

    vector<tuple<char, char, int> > tree;
    while(tree.size() < verteices.size()-1) {
        const auto& input_edge = edges.front();

        if(!is_Cycle(tree, get<0>(input_edge), get<1>(input_edge))) tree.push_back(input_edge);
        edges.erase(edges.begin());
    }

    return tree;
}

int main() {
    ifstream input("input_Kruskal.txt");

    if(!input.is_open()) {
        cerr << "파일을 열 수 없음\n";
        return 0;
    }

    vector<tuple<char, char, int> > edges;
    set<char> verteices;
    string line;

    while(getline(input, line)) {
        line.erase(remove(line.begin(), line.end(), '('), line.end());
        line.erase(remove(line.begin(), line.end(), ')'), line.end());

        stringstream ss(line);
        char vertex1, vertex2;
        int weight;
        char comma;

        ss >> vertex1 >> comma >> vertex2 >> comma >> weight;
        edges.push_back(make_tuple(vertex1, vertex2, weight));
        verteices.insert(vertex1);  verteices.insert(vertex2);
    }
    input.close();

    clock_t start = clock();
    vector<tuple<char, char, int> > tree = KrusalMST(edges, verteices);
    clock_t finish = clock();

    for (const auto& edge : tree) printf("(%c, %c, %d)\n", get<0>(edge), get<1>(edge), get<2>(edge));
    printf("running time (ms): %f\n", (double)(finish-start));

    return 0;
}