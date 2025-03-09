#include <iostream>
#include <vector>
#include <windows.h>
#include <cstdio>
#include <algorithm>
#include <functional>
#include <map>
#include <set>

using namespace std;

class Graph
{
public:
    int V;
    map<int, char> vertice_para_char;
    vector<vector<int>> lista_adjacencia;

    Graph(int vertices) : V(vertices)
    {
        lista_adjacencia.resize(V);
        for (int i = 0; i < V; i++)
        {
            vertice_para_char[i] = 'A' + i;
        }
    }

    // Função para adicionar uma aresta ao grafo não direcionado
    void adicionar_aresta(int src, int dest)
    {
        lista_adjacencia[src].push_back(dest);
        lista_adjacencia[dest].push_back(src); // Grafo não direcionado
    }

    // Função auxiliar para encontrar ciclos usando DFS
    void encontra_ciclos(int u, int pai, vector<bool> &visitado,
                         vector<int> &path, vector<vector<int>> &ciclos)
    {
        visitado[u] = true;
        path.push_back(u);

        // Verificar todos os vértices lista_adjacencia
        for (int v : lista_adjacencia[u])
        {
            if (!visitado[v])
            {
                encontra_ciclos(v, u, visitado, path, ciclos);
            }
            // Se o vértice já foi visitado e não é o pai do vértice atual, temos um ciclo
            else if (v != pai && find(path.begin(), path.end(), v) != path.end())
            {
                vector<int> ciclo;
                auto it = find(path.begin(), path.end(), v);
                while (it != path.end())
                {
                    ciclo.push_back(*it);
                    ++it;
                }

                // Verificar se este ciclo já foi encontrado (com rotação diferente)
                bool eh_novo = true;
                for (const auto &ciclo_existente : ciclos)
                {
                    if (ciclo_existente.size() == ciclo.size())
                    {
                        vector<int> tempciclo = ciclo;
                        tempciclo.insert(tempciclo.end(), tempciclo.begin(), tempciclo.end());
                        for (size_t i = 0; i < ciclo.size(); i++)
                        {
                            vector<int> rotacao(tempciclo.begin() + i, tempciclo.begin() + i + ciclo.size());
                            vector<int> rotacao_inversa = rotacao;
                            reverse(rotacao_inversa.begin(), rotacao_inversa.end());

                            if (rotacao == ciclo_existente || rotacao_inversa == ciclo_existente)
                            {
                                eh_novo = false;
                                break;
                            }
                        }
                    }
                    if (!eh_novo)
                        break;
                }
                if (eh_novo)
                {
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
    vector<vector<int>> enumera_ciclos()
    {
        vector<bool> visitado(V, false);
        vector<int> path;
        vector<vector<int>> ciclos;

        for (int i = 0; i < V; i++)
        {
            if (!visitado[i])
            {
                encontra_ciclos(i, -1, visitado, path, ciclos);
            }
        }
        return ciclos;
    }

    // função auxiliar para encontrar ciclos usando permutação
    void encontrar_ciclos_permutacao(vector<vector<int>> &ciclos)
    {
        vector<int> vertices(V);
        for (int i = 0; i < V; i++)
        {
            vertices[i] = i;
        }

        // looping para realizar a permutação sobre todos os vertices e seus possíveis ciclos
        do
        {
            for (int tamanho = 3; tamanho <= V; tamanho++)
            {
                // geração de sub-grafos para verificação de ciclos
                vector<int> sub(vertices.begin(), vertices.begin() + tamanho);
                if (forma_ciclo(sub))
                {
                    normalizar_ciclo(sub);
                    ciclos.push_back(sub);
                }
            }
        } while (next_permutation(vertices.begin(), vertices.end()));
    }

    // função para verificar a existencia de um ciclo dentro do sub-grafo em questão
    bool forma_ciclo(const vector<int> &ciclo)
    {
        for (size_t i = 0; i < ciclo.size(); i++)
        {
            int u = ciclo[i];
            int v = ciclo[(i + 1) % ciclo.size()];

            //utilização da lista de adjacencia para verificar a existência de arestas entre os vertices, inclusive entre o primeiro e último para garantia da existência do ciclo 
            if (find(lista_adjacencia[u].begin(), lista_adjacencia[u].end(), v) == lista_adjacencia[u].end())
            {
                return false;
            }
        }
        return true;
    }

    // Função auxiliar para encontrar a menor rotação lexicográfica de um ciclo
    vector<int> menor_rotacao(const vector<int> &ciclo)
    {
        int n = ciclo.size();
        vector<int> menor = ciclo;

        // Testar todas as rotações do ciclo e guardar a menor
        for (int i = 1; i < n; i++)
        {
            vector<int> temp(ciclo.begin() + i, ciclo.end());
            temp.insert(temp.end(), ciclo.begin(), ciclo.begin() + i); // Rotacionar

            if (temp < menor)
            {
                menor = temp;
            }
        }
        return menor;
    }

    // Função para normalizar um ciclo, para posterior verificação de ciclos em duplicata
    void normalizar_ciclo(vector<int> &ciclo)
    {
        // Encontrar a menor rotação lexicográfica do ciclo
        vector<int> menor_ciclo = menor_rotacao(ciclo);

        // Inverter o ciclo e encontrar sua menor rotação lexicográfica
        vector<int> ciclo_invertido = ciclo;
        reverse(ciclo_invertido.begin(), ciclo_invertido.end());
        vector<int> menor_ciclo_invertido = menor_rotacao(ciclo_invertido);

        // Escolher a menor entre os dois
        ciclo = min(menor_ciclo, menor_ciclo_invertido);
    }

    void imprime_com_letras(const vector<vector<int>> &ciclos)
    {
        //looping para eliminar ciclos em duplicata no vetor
        set<vector<int>> ciclos_unicos;
        for (const auto &ciclo : ciclos)
        {
            ciclos_unicos.insert(ciclo);
        }

        cout << "Ciclos encontrados: " << ciclos_unicos.size() << endl;
        for (const auto &ciclo : ciclos_unicos)
        {
            cout << "Ciclo: ";
            for (int vertice : ciclo)
            {
                cout << vertice_para_char[vertice] << " ";
            }
            cout << endl;
        }
    }
};

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    Graph g(6);
    //-//
    g.adicionar_aresta(0, 1);
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

    cout << "Ciclos Caminhamento:" << endl;
    vector<vector<int>> ciclos_caminhamento = g.enumera_ciclos();
    g.imprime_com_letras(ciclos_caminhamento);

    cout << "\nCiclos Permutação:" << endl;
    vector<vector<int>> ciclos_permutacao;
    g.encontrar_ciclos_permutacao(ciclos_permutacao);
    g.imprime_com_letras(ciclos_permutacao);

    // Mapeamento para referência
    cout << "\nMapeamento de vértices:" << endl;
    for (int i = 0; i < g.V; i++)
    {
        cout << i << " -> " << g.vertice_para_char[i] << endl;
    }
    return 0;
}
