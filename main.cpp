#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();

// Convert char to cost
int charToCost(char c) {
    if ('A' <= c && c <= 'Z') return c - 'A';
    return c - 'a' + 26;
}

// Disjoint Set Union-Find
class UnionFind {
public:
    vector<int> parent;
    UnionFind(int n) {
        parent.resize(n);
        for (int i = 0; i < n; ++i)
            parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false;
        parent[py] = px;
        return true;
    }
};

// Edge structure
struct Edge {
    int u, v, cost;
    bool exists; // true = existing edge (destruction), false = new edge (construction)
};

int main() {
    string countryStr, buildStr, destroyStr;
    getline(cin, countryStr);
    getline(cin, buildStr);
    getline(cin, destroyStr);

    // Parse input
    vector<string> countryRows, buildRows, destroyRows;
    int pos = 0, n = 0;

    auto split = [](const string& s, char delim) {
        vector<string> result;
        string part;
        for (char c : s) {
            if (c == delim) {
                result.push_back(part);
                part.clear();
            } else {
                part += c;
            }
        }
        result.push_back(part);
        return result;
    };

    countryRows = split(countryStr, ',');
    buildRows = split(buildStr, ',');
    destroyRows = split(destroyStr, ',');

    n = countryRows.size();
    vector<vector<int>> country(n, vector<int>(n));
    vector<vector<int>> build(n, vector<int>(n));
    vector<vector<int>> destroy(n, vector<int>(n));

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            country[i][j] = countryRows[i][j] - '0';
            build[i][j] = charToCost(buildRows[i][j]);
            destroy[i][j] = charToCost(destroyRows[i][j]);
        }

    // Prepare edge list
    vector<Edge> edges;
    int totalDestroyCost = 0;

    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j) {
            if (country[i][j] == 1) {
                // Existing edge: can destroy
                edges.push_back({i, j, destroy[i][j], true});
                totalDestroyCost += destroy[i][j];
            } else {
                // Potential new edge
                edges.push_back({i, j, build[i][j], false});
            }
        }

    // Sort edges: cheaper cost first
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.cost < b.cost;
    });

    UnionFind uf(n);
    int savedCost = 0;

    for (const Edge& e : edges) {
        if (uf.unite(e.u, e.v)) {
            // If they were not connected, we must keep this edge
            if (e.exists) {
                // If it existed, don't destroy — we saved the cost
                savedCost += e.cost;
            } else {
                // If it's a build, we must pay the cost
                totalDestroyCost += e.cost;
            }
        }
    }

    int result = totalDestroyCost - savedCost;
    cout << result << endl;

    return 0;
}
