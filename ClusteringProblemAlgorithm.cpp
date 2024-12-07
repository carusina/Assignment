#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <set>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

double euclidean_distance(pair<int, int> p1, pair<int, int> p2) {
    return sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2));
}

void Approx_k_Clusters(vector<pair<int, int> >& points, int k) {
    vector<pair<int, int> > unique_points;
    set<pair<int, int> > seen;

    for(const auto& point : points) {
        if(seen.find(point) == seen.end()) {
            unique_points.push_back(point);
            seen.insert(point);
        }
    }

    int n = unique_points.size();
    vector<pair<int, int> > centers;
    centers.push_back(unique_points[0]);

    vector<int> cluster_assignment(n, -1);

    for(int j = 1; j < k; j++) {
        vector<double> distances(n, numeric_limits<double>::max());

        for(int i = 0; i < n; i++) {
            if(find(centers.begin(), centers.end(), points[i]) != centers.end()) {
                continue;
            }

            for(auto& center : centers) {
                double dis = euclidean_distance(unique_points[i], center);
                distances[i] = min(distances[i], dis); 
            }
        }

        double max_distance = -1;
        int max_index = -1;
        for(int i = 0; i < n; i++) {
            if(find(centers.begin(), centers.end(), points[i]) == centers.end() && distances[i] > max_distance) {
                max_distance = distances[i];
                max_index = i;
            }
        }

        if(max_index != -1) {
            centers.push_back(unique_points[max_index]);
        }
    }

    map<int, vector<pair<int, int> > > clusters;
    for(int i = 0; i < n; i++) {
        double min_distance = numeric_limits<double>::max();
        int closest_center = -1;

        for(int c = 0; c < k; c++) {
            double dis = euclidean_distance(unique_points[i], centers[c]);
            if(dis < min_distance) {
                min_distance = dis;
                closest_center = c;
            }
        }

        cluster_assignment[i] = closest_center;
        clusters[closest_center].push_back(unique_points[i]);
    }

    for(int i = 0; i < k; i++) {
        printf("Cluster of Center(%2d, %2d): ", centers[i].first, centers[i].second);
        for(auto& point : clusters[i]) {
            printf("(%2d, %2d), ", point.first, point.second);
        }
        printf("\n");
    }
}

int main() {
    vector<pair<int, int> > points;
    string  filename = "clustering_input.txt";

    ifstream input(filename);
    if(!input.is_open()) {
        cerr << filename << "을 열 수 없음";
        return 1;
    }

    string line;
    while(getline(input, line)) {
        istringstream iss(line);
        int x, y;
        if(iss >> x >> y) {
            points.emplace_back(x, y);
        }
    }
    input.close();

    int k = 8;
    Approx_k_Clusters(points, 8);
    return 0;
}