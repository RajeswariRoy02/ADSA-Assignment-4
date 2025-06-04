#include <iostream>
#include <vector>
#include <limits>
#include <string>
#include <sstream>

using namespace std;

const int INF = numeric_limits<int>::max();

int charToCost(char c) {
    if ('A' <= c && c <= 'Z') return c - 'A';
    return c - 'a' + 26;
}

vector<string> splitLine(const string& line) {
    vector<string> result;
    stringstream ss(line);
    string token;
    while (getline(ss, token, ',')) {
        result.push_back(token);
    }
    return result;
}

int main() {
    string countryLine, buildLine, destroyLine;
    getline(cin, countryLine);
    getline(cin, buildLine);
    getline(cin, destroyLine);

    vector<string> countryRows = splitLine(countryLine);
    vector<string> buildRows = splitLine(buildLine);
    vector<string> destroyRows = splitLine(destroyLine);

    int n = countryRows.size();

    vector<vector<int>> dist(n, vector<int>(n, INF));
    vector<vector<int>> build(n, vector<int>(n, INF));
    vector<vector<int>> destroy(n, vector<int>(n, INF));

    // Parse the country graph
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (countryRows[i][j] == '1') {
                dist[i][j] = 0; // Already connected, no cost
            }
        }
    }

    // Parse build and destroy costs
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            build[i][j] = charToCost(buildRows[i][j]);
            destroy[i][j] = charToCost(destroyRows[i][j]);
        }
    }

    // Floyd-Warshall
    for (int k = 0; k < n; ++k)
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                if (dist[i][k] != INF && dist[k][j] != INF)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);

    // Build minimum spanning connections
    int totalCost = 0;
    vector<vector<bool>> used(n, vector<bool>(n, false));

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (countryRows[i][j] == '1') {
                if (dist[i][j] != 0) {
                    totalCost += destroy[i][j]; // Destroy unnecessary connection
                }
            } else {
                if (dist[i][j] == INF) {
                    totalCost += build[i][j]; // Need to build a connection
                }
            }
        }
    }

    cout << totalCost << endl;
    return 0;
}
