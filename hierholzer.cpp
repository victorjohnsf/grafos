#include <bits/stdc++.h>
using namespace std;

struct Aresta {
    int destino;
    int id;
    bool usada = false;
};

using Grafo = vector<vector<Aresta>>;

void adicionarArestaNaoDirecionada(Grafo& g, int u, int v, int idAresta) {
    g[u].push_back({v, idAresta, false});
    g[v].push_back({u, idAresta, false});
}

vector<int> circuitoEuleriano(const Grafo& original) {
    int n = (int)original.size();
    Grafo g = original;
    int inicio = 0;
    while (inicio < n && g[inicio].empty()) ++inicio;
    if (inicio == n) return {};

    vector<int> circuito;
    vector<int> pilha;
    pilha.push_back(inicio);

    while (!pilha.empty()) {
        int v = pilha.back();
        while (!g[v].empty() && g[v].back().usada)
            g[v].pop_back();

        if (g[v].empty()) {
            circuito.push_back(v);
            pilha.pop_back();
        } else {
            Aresta a = g[v].back();
            g[v].pop_back();
            auto& listaAdj = g[a.destino];
            for (auto it = listaAdj.rbegin(); it != listaAdj.rend(); ++it) {
                if (it->id == a.id && !it->usada) {
                    it->usada = true;
                    break;
                }
            }
            a.usada = true;
            pilha.push_back(a.destino);
        }
    }
    reverse(circuito.begin(), circuito.end());
    return circuito;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Uso: " << argv[0] << " <arquivo_entrada> <arquivo_saida>\n";
        return 1;
    }

    string caminhoEntrada = argv[1];
    string caminhoSaida   = argv[2];

    ifstream arquivoEntrada(caminhoEntrada);
    if (!arquivoEntrada) {
        cerr << "Erro: nao foi possivel abrir o arquivo de entrada '" << caminhoEntrada << "'\n";
        return 1;
    }

    int nVertices, nArestas;
    if (!(arquivoEntrada >> nVertices >> nArestas)) {
        cerr << "Erro: falha ao ler o numero de vertices e arestas\n";
        return 1;
    }
    if (nVertices < 0 || nArestas < 0) {
        cerr << "Erro: quantidade de vertices/arestas deve ser nao-negativa\n";
        return 1;
    }

    Grafo g(nVertices);
    vector<int> grau(nVertices, 0);

    for (int i = 0; i < nArestas; ++i) {
        int u, v;
        if (!(arquivoEntrada >> u >> v)) {
            cerr << "Erro: dados de arestas insuficientes (esperado " << nArestas << " linhas)\n";
            return 1;
        }
        if (u < 0 || u >= nVertices || v < 0 || v >= nVertices) {
            cerr << "Erro: indice de vertice fora do intervalo na aresta " << i << "\n";
            return 1;
        }
        adicionarArestaNaoDirecionada(g, u, v, i);
        ++grau[u];
        ++grau[v];
    }
    arquivoEntrada.close();

    for (int v = 0; v < nVertices; ++v)
        if (grau[v] % 2 != 0) {
            cerr << "Erro: o grafo nao e Euleriano – vertice " << v
                 << " possui grau impar (" << grau[v] << ")\n";
            return 1;
        }

    vector<int> circuito = circuitoEuleriano(g);

    if ((int)circuito.size() != nArestas + 1) {
        cerr << "Aviso: tamanho do circuito retornado (" << circuito.size()
             << ") nao corresponde ao esperado (" << nArestas + 1 << ")\n";
    }

    ofstream arquivoSaida(caminhoSaida);
    if (!arquivoSaida) {
        cerr << "Erro: nao foi possivel abrir o arquivo de saida '" << caminhoSaida << "'\n";
        return 1;
    }
    for (size_t i = 0; i < circuito.size(); ++i) {
        arquivoSaida << circuito[i];
        if (i + 1 < circuito.size()) arquivoSaida << ' ';
    }
    arquivoSaida << '\n';
    arquivoSaida.close();

    cout << "Circuito euleriano gravado em '" << caminhoSaida << "'\n";
    return 0;
}