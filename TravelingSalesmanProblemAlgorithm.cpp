#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <cmath>
#include <unordered_set>
#include <algorithm>

using namespace std;

typedef tuple<char, int, int> Vertex;
typedef tuple<double, char, char> Edge;

double calculateDistance(const Vertex& a, const Vertex& b) {
    int x1 = get<1>(a), y1 = get<2>(a);
    int x2 = get<1>(b), y2 = get<2>(b);
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

vector<Edge> PrimMST(const vector<Vertex>& vertices) {
    unordered_set<char> visited;
    vector<Edge> mstEdges;
    auto compare = [](const Edge& a, const Edge& b) {
        return get<0>(a) > get<0>(b);
    };
    priority_queue<Edge, vector<Edge>, decltype(compare)> minHeap(compare);

    char start = get<0>(vertices[0]);
    visited.insert(start);

    for (const auto& vertex : vertices) {
        if (get<0>(vertex) != start) {
            double distance = calculateDistance(vertices[0], vertex);
            minHeap.push(make_tuple(distance, start, get<0>(vertex)));
        }
    }

    while (visited.size() < vertices.size()) {
        Edge minEdge = minHeap.top();
        minHeap.pop();

        double weight = get<0>(minEdge);
        char u = get<1>(minEdge);
        char v = get<2>(minEdge);

        if (visited.find(v) != visited.end()) {
            continue;
        }

        visited.insert(v);
        mstEdges.push_back(minEdge);

        for (const auto& vertex : vertices) {
            char w = get<0>(vertex);
            if (visited.find(w) == visited.end()) {
                double distance = calculateDistance(vertex, *find_if(vertices.begin(), vertices.end(), [&](const Vertex& x) {
                    return get<0>(x) == v;
                }));
                minHeap.push(make_tuple(distance, v, w));
            }
        }
    }

    return mstEdges;
}

void TSP_Helper(vector<char>& visited, const vector<Edge>& MST, char start) {
    for(const auto& edge : MST) {
        char u = get<1>(edge);
        char v = get<2>(edge);

        if(u == start && find(visited.begin(), visited.end(), v) == visited.end()) {
            visited.push_back(v);
            TSP_Helper(visited, MST, v);
        } else if(v == start && find(visited.begin(), visited.end(), u) == visited.end()) {
            visited.push_back(u);
            TSP_Helper(visited, MST, u);
        }
    }
}

void Approx_MST_TSP(const vector<Vertex>& vertices, const vector<Edge>& MST, char start) {
    vector<char> visited;
    visited.push_back(start);

    TSP_Helper(visited, MST, start);
    visited.push_back(start);

    for(char vertex : visited) {
        printf("%c ", vertex);
    }

    double distance_sum = 0;
    for(int i = 0; i < visited.size()-1; i++) {
        Vertex A;
        Vertex B;

        for(Vertex vertex : vertices) {
            if(get<0>(vertex) == visited[i]) {
                A = vertex;
            }
            else if(get<0>(vertex) == visited[i+1]) {
                B = vertex;
            }
        }
        // printf("%c to %c \t %lf\n",get<0>(A), get<0>(B), calculateDistance(A, B));
        distance_sum += calculateDistance(A, B);
    }
    printf("\t %lf\n", distance_sum);
} 

int main() {
    vector<Vertex> vertices = {
        make_tuple('A', 0, 3),
        make_tuple('B', 7, 5),
        make_tuple('C', 6, 0),
        make_tuple('D', 4, 3),
        make_tuple('E', 1, 0),
        make_tuple('F', 5, 3),
        make_tuple('H', 4, 1),
        make_tuple('G', 2, 2)
    };

    vector<Edge> MST = PrimMST(vertices);
    Approx_MST_TSP(vertices, MST, 'A');

    return 0;
}
