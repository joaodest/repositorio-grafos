#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <set>

using namespace std;

class Graph {
public:
    int V;
    map<int, char> vertice_para_char;
    vector<vector<int>> lista_adjacencia;

    Graph(int vertices) : V(vertices) {
        lista_adjacencia.resize(V);
        for (int i = 0; i < V; i++) {
            vertice_para_char[i] = 'A' + i;
        }
    }

    void adicionar_aresta(int src, int dest) {
        lista_adjacencia[src].push_back(dest);
        lista_adjacencia[dest].push_back(src);
    }

    void encontrar_ciclos(vector<vector<int>>& ciclos) {
        vector<int> vertices(V);
        for (int i = 0; i < V; i++) {
            vertices[i] = i;
        }
        
        do {
            for (int tamanho = 3; tamanho <= V; tamanho++) {
                vector<int> sub(vertices.begin(), vertices.begin() + tamanho);
                if (forma_ciclo(sub)) {
                    normalizar_ciclo(sub);
                    ciclos.push_back(sub);
                }
            }
        } while (next_permutation(vertices.begin(), vertices.end()));
    }

    bool forma_ciclo(const vector<int>& ciclo) {
        for (size_t i = 0; i < ciclo.size(); i++) {
            int u = ciclo[i];
            int v = ciclo[(i + 1) % ciclo.size()];
            if (find(lista_adjacencia[u].begin(), lista_adjacencia[u].end(), v) == lista_adjacencia[u].end()) {
                return false;
            }
        }
        return true;
    }

    void normalizar_ciclo(vector<int>& ciclo) {
        rotate(ciclo.begin(), min_element(ciclo.begin(), ciclo.end()), ciclo.end());
        if (ciclo[1] > ciclo.back()) {
            reverse(ciclo.begin(), ciclo.end());
        }
    }

    void imprime_com_letras(const vector<vector<int>>& ciclos) {
        set<vector<int>> ciclos_unicos;
        for (const auto& ciclo : ciclos) {
            ciclos_unicos.insert(ciclo);
        }

        cout << "Ciclos encontrados: " << ciclos_unicos.size() << endl;
        for (const auto& ciclo : ciclos_unicos) {
            cout << "Ciclo: ";
            for (int vertice : ciclo) {
                cout << vertice_para_char[vertice] << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    Graph g(6);
    g.adicionar_aresta(0, 1);
    g.adicionar_aresta(0, 2);
    g.adicionar_aresta(0, 3);
    g.adicionar_aresta(0, 4);
    g.adicionar_aresta(1, 3);
    g.adicionar_aresta(1, 2);
    g.adicionar_aresta(1, 4);
    g.adicionar_aresta(2, 3);
    g.adicionar_aresta(2, 4);
    g.adicionar_aresta(2, 5);
    g.adicionar_aresta(3, 5);
    g.adicionar_aresta(4, 5);

    vector<vector<int>> ciclos;
    g.encontrar_ciclos(ciclos);
    g.imprime_com_letras(ciclos);
    
    return 0;
}
