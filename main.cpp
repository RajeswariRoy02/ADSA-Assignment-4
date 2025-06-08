#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

int charToCost(char c) {
    return (c >= 'A' && c <= 'Z') ? (c - 'A') : (c - 'a' + 26);
}

vector<string> split(const string& s, char delim) {
    vector<string> res;
    string temp;
    for (char c : s) {
        if (c == delim) {
            res.push_back(temp);
            temp.clear();
        } else {
            temp += c;
        }
    }
    res.push_back(temp);
    return res;
}

class UnionFind {
public:
    vector<int> parent;
    UnionFind(int n) {
        parent.resize(n);
        for (int i = 0; i < n; ++i) parent[i] = i;
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

struct Edge {
    int u, v, cost;
    bool existing;
};

int main() {
    string line;
    getline(cin, line);

    vector<string> parts = split(line, ' ');
    vector<string> countryRaw = split(parts[0], ',');
    vector<string> buildRaw = split(parts[1], ',');
    vector<string> destroyRaw = split(parts[2], ',');

    int n = countryRaw.size();
    vector<vector<int>> country(n, vector<int>(n));
    vector<vector<int>> build(n, vector<int>(n));
    vector<vector<int>> destroy(n, vector<int>(n));

    vector<Edge> edges;
    int totalDestroyCost = 0;

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            country[i][j] = countryRaw[i][j] - '0';
            build[i][j] = charToCost(buildRaw[i][j]);
            destroy[i][j] = charToCost(destroyRaw[i][j]);
        }

    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j) {
            if (country[i][j]) {
                totalDestroyCost += destroy[i][j];
                edges.push_back({i, j, -destroy[i][j], true});
            } else {
                edges.push_back({i, j, build[i][j], false});
            }
        }

    sort(edges.begin(), edges.end(), [](Edge a, Edge b) {
        return a.cost < b.cost;
    });

    UnionFind uf(n);
    int resultCost = totalDestroyCost;

    for (auto& e : edges) {
        if (uf.unite(e.u, e.v)) {
            resultCost += e.cost;
        }
    }

    cout << resultCost << endl;
    return 0;
}
