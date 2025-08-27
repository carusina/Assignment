// must nedded cmd option '-std=c++11'

#include <iostream>
#include <vector>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();

vector<vector<int> > graph;
vector<char> nodes = {'A', 'B', 'C', 'D', 'E'};
int num_nodes = nodes.size();

vector<bool> visited;
vector<int> path;
int best_distance = INF;
vector<int> best_path;

void initialize_graph() {
    graph = vector<vector<int> >(num_nodes, vector<int>(num_nodes, INF));

    graph[0][1] = 2; graph[0][2] = 7; graph[0][3] = 3; graph[0][4] = 10;
    graph[1][2] = 3; graph[1][3] = 5; graph[1][4] = 4;
    graph[2][3] = 6; graph[2][4] = 1;
    graph[3][4] = 9;

    for(int i = 0; i < num_nodes; i++) {
        for(int j = 0; j <num_nodes; j++) {
            if(graph[i][j] != INF) {
                graph[j][i] = graph[i][j];
            }
        }
    }
}

void BacktrackTSP(int current_node, int current_distance, int level) {
    if(level == num_nodes) {
        if(graph[current_node][0] != INF) {
            int total_distance = current_distance + graph[current_node][0];

            if(total_distance < best_distance) {
                best_distance = total_distance;
                best_path = path;
                best_path.push_back(0);
            }
        }
        return;
    }

    for(int next_node = 0; next_node < num_nodes; next_node++) {
        if(!visited[next_node] && graph[current_node][next_node] != INF) {
            visited[next_node] = true;
            path.push_back(next_node);
            BacktrackTSP(next_node, current_distance + graph[current_node][next_node], level+1);
            path.pop_back();
            visited[next_node] = false;
        }
    }
}

int main() {
    initialize_graph();

    visited = vector<bool>(num_nodes, false);
    visited[0] = true;
    path.push_back(0);

    BacktrackTSP(0, 0, 1);

    printf("[ ");
    for (int i : best_path) {
        printf("%c ", nodes[i]);
    }
    printf("], Distance: %d\n", best_distance);

    return 0;
}