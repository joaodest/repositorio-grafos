#include <iostream>
#include <vector>
#include <list>
#include <windows.h>

class Grafo {
private:
    int numero_vertices;
    std::vector<std::list<int>> lista_adjacencia;

public:
    Grafo(int numero_vertices) : numero_vertices(numero_vertices) {
        lista_adjacencia.resize(numero_vertices);
    }

    void adicionarAresta(int v1, int v2) {
        lista_adjacencia[v1].push_back(v2);
    }

    void imprimir() {
        for (int v = 0; v < numero_vertices; v++) {
            std::cout << "Vertice " << v << ": ";
            for (int u : lista_adjacencia[v]) {
                std::cout << u << " ";
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    SetConsoleOutputCP(CP_UTF8);
    Grafo g(4);

    g.adicionarAresta(0, 1);
    g.adicionarAresta(2, 3);
    g.adicionarAresta(3, 1);
    g.adicionarAresta(3, 3);

    std::cout << "Representacao do grafo (lista de adjacencia):\n";
    g.imprimir();

    return 0;
}
