#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to;
    int id;
    bool used = false;
};

using Graph = vector<vector<Edge>>;

void addUndirectedEdge(Graph& g, int u, int v, int eid) {
    g[u].push_back({v, eid, false});
    g[v].push_back({u, eid, false});
}

vector<int> eulerianCircuit(const Graph& original) {
    int n = (int)original.size();
    Graph g = original;
    int start = 0;
    while (start < n && g[start].empty()) ++start;
    if (start == n) return {};

    vector<int> circuit;
    vector<int> stack;
    stack.push_back(start);

    while (!stack.empty()) {
        int v = stack.back();
        while (!g[v].empty() && g[v].back().used)
            g[v].pop_back();

        if (g[v].empty()) {
            circuit.push_back(v);
            stack.pop_back();
        } else {
            Edge e = g[v].back();
            g[v].pop_back();
            auto& list = g[e.to];
            for (auto it = list.rbegin(); it != list.rend(); ++it) {
                if (it->id == e.id && !it->used) {
                    it->used = true;
                    break;
                }
            }
            e.used = true;
            stack.push_back(e.to);
        }
    }
    reverse(circuit.begin(), circuit.end());
    return circuit;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>\n";
        return 1;
    }

    string inPath = argv[1];
    string outPath = argv[2];

    ifstream fin(inPath);
    if (!fin) {
        cerr << "Error: cannot open input file '" << inPath << "'\n";
        return 1;
    }

    int nV, nE;
    if (!(fin >> nV >> nE)) {
        cerr << "Error: failed to read number of vertices and edges\n";
        return 1;
    }
    if (nV < 0 || nE < 0) {
        cerr << "Error: vertex/edge count must be non-negative\n";
        return 1;
    }

    Graph g(nV);
    vector<int> degree(nV, 0);

    for (int i = 0; i < nE; ++i) {
        int u, v;
        if (!(fin >> u >> v)) {
            cerr << "Error: insufficient edge data (expected " << nE << " lines)\n";
            return 1;
        }
        if (u < 0 || u >= nV || v < 0 || v >= nV) {
            cerr << "Error: vertex index out of range in edge " << i << "\n";
            return 1;
        }
        addUndirectedEdge(g, u, v, i);
        ++degree[u];
        ++degree[v];
    }
    fin.close();

    for (int v = 0; v < nV; ++v)
        if (degree[v] % 2 != 0) {
            cerr << "Error: graph is not Eulerian – vertex " << v
                 << " has odd degree (" << degree[v] << ")\n";
            return 1;
        }

    vector<int> circuit = eulerianCircuit(g);

    if ((int)circuit.size() != nE + 1) {
        cerr << "Warning: returned circuit length (" << circuit.size()
             << ") does not match expected (" << nE + 1 << ")\n";
    }

    ofstream fout(outPath);
    if (!fout) {
        cerr << "Error: cannot open output file '" << outPath << "'\n";
        return 1;
    }
    for (size_t i = 0; i < circuit.size(); ++i) {
        fout << circuit[i];
        if (i + 1 < circuit.size()) fout << ' ';
    }
    fout << '\n';
    fout.close();

    cout << "Eulerian circuit written to '" << outPath << "'\n";
    return 0;
}
