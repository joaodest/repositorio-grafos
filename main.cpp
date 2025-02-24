#include <iostream>
#include <vector>
#include <windows.h>
#include <cstdio>
#include <algorithm>
#include <functional>
#include <map>

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

    // Função para adicionar uma aresta ao grafo não direcionado
    void adicionar_aresta(int src, int dest) {
        lista_adjacencia[src].push_back(dest);
        lista_adjacencia[dest].push_back(src); // Grafo não direcionado
    }

    // Função auxiliar para encontrar ciclos usando DFS
    void encontra_ciclos(int u, int pai, vector<bool>& visitado,
                      vector<int>& path, vector<vector<int>>& ciclos) {
        visitado[u] = true;
        path.push_back(u);

        // Verificar todos os vértices lista_adjacencia
        for (int v : lista_adjacencia[u]) {
            if (!visitado[v]) {
                encontra_ciclos(v, u, visitado, path, ciclos);
            }
            // Se o vértice já foi visitado e não é o pai do vértice atual, temos um ciclo
            else if (v != pai && find(path.begin(), path.end(), v) != path.end()) {
                vector<int> ciclo;
                auto it = find(path.begin(), path.end(), v);
                while (it != path.end()) {
                    ciclo.push_back(*it);
                    ++it;
                }

                // Verificar se este ciclo já foi encontrado (com rotação diferente)
                bool eh_novo = true;
                for (const auto& ciclo_existente : ciclos) {
                    if (ciclo_existente.size() == ciclo.size()) {
                        vector<int> tempciclo = ciclo;
                        tempciclo.insert(tempciclo.end(), tempciclo.begin(), tempciclo.end());
                        for (size_t i = 0; i < ciclo.size(); i++) {
                            vector<int> rotacao(tempciclo.begin() + i, tempciclo.begin() + i + ciclo.size());
                            vector<int> rotacao_inversa = rotacao;
                            reverse(rotacao_inversa.begin(), rotacao_inversa.end());

                            if (rotacao == ciclo_existente || rotacao_inversa == ciclo_existente) {
                                eh_novo = false;
                                break;
                            }
                        }
                    }
                    if (!eh_novo) break;
                }
                if (eh_novo) {
                    ciclos.push_back(ciclo);
                }
            }
        }
        // Remover o vértice atual do caminho e marcar como não visitado
        // para permitir que outros DFS encontrem ciclos diferentes
        path.pop_back();
        visitado[u] = false;
    }

    // Função para enumerar todos os ciclos únicos
    vector<vector<int>> enumera_ciclos() {
        vector<bool> visitado(V, false);
        vector<int> path;
        vector<vector<int>> ciclos;

        for (int i = 0; i < V; i++) {
            if (!visitado[i]) {
                encontra_ciclos(i, -1, visitado, path, ciclos);
            }
        }
        return ciclos;
    }
    
    void imprime_com_letras(const vector<vector<int>>& ciclos) {
        cout << "Ciclos encontrados:" << endl;
        for (const auto& ciclo : ciclos) {
            cout << "Ciclo: ";
            for (int vertice : ciclo) {
                cout << vertice_para_char[vertice] << " ";
            }
            cout << endl;
        }
    }
};


    int main() {
        SetConsoleOutputCP(CP_UTF8);


        Graph g(6);
         //-//
         g.adicionar_aresta(0, 1);
         g.adicionar_aresta(0, 2);
         g.adicionar_aresta(0, 3);
         g.adicionar_aresta(0, 4);
         //-//
         g.adicionar_aresta(1, 3);
         g.adicionar_aresta(1, 2);
         g.adicionar_aresta(1, 4);
         //-//
         g.adicionar_aresta(2, 3);
         g.adicionar_aresta(2, 4);
         g.adicionar_aresta(2, 5);
         //-//
         g.adicionar_aresta(3, 5);
         //-//
         g.adicionar_aresta(4, 5);

        vector<vector<int>> ciclos = g.enumera_ciclos();

        g.imprime_com_letras(ciclos);

        // Mapeamento para referência
        cout << "\nMapeamento de vértices:" << endl;
        for (int i = 0; i < g.V; i++) {
            cout << i << " -> " << g.vertice_para_char[i] << endl;
        }        
        return 0;
    }

