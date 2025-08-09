#include "Gerenciador.h"
#include "Gulosos.h"
#include <fstream>
#include <limits>

void Gerenciador::imprimirResultadoDFS(Grafo *arvore)
{
    cout << "========== ARVORE DE CAMINHAMENTO EM PROFUNDIDADE ==========" << endl;
    arvore->imprimirGrafo(); // imprime a estrutura principal da árvore usando a função genérica

    cout << "========== ARESTAS DA ARVORE ==========" << endl;
    if (arvore->arestas_da_arvore_dfs.empty())
    {
        cout << "Nenhuma aresta encontrada." << endl;
    }
    else
    {
        for (const auto &aresta : arvore->arestas_da_arvore_dfs)
        {
            cout << "Aresta: " << aresta.first << " -> " << aresta.second << endl;
        }
    }

    cout << "=========================================" << endl
         << endl;
}

void Gerenciador::comandos(Grafo *grafo)
{

    cout << "Digite uma das opcoes abaixo e pressione enter:" << endl
         << endl;
    cout << "(a) Fecho transitivo direto de um no;" << endl;
    cout << "(b) Fecho transitivo indireto de um no;" << endl;
    cout << "(c) Caminho minimo (Djikstra);" << endl;
    cout << "(d) Caminho minimo (Floyd);" << endl;
    cout << "(e) Arvore Geradora Minima (Algoritmo de Prim);" << endl;
    cout << "(f) Arvore Geradora Minima (Algoritmo de Kruskal);" << endl;
    cout << "(g) Arvore de caminhamento em profundidade;" << endl;
    cout << "(h) Raio, diametro, centro e periferia do grafo;" << endl;
    cout << "(i) Algoritmo guloso simples;" << endl;
    cout << "(j) Algoritmo guloso randomizado adaptativo;" << endl;
    cout << "(k) Algoritmo guloso randomizado adaptativo reativo;" << endl;
    cout << "(0) Sair;" << endl
         << endl;

    char resp;
    cin >> resp;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    switch (resp)
    {
    case 'a':
    {

        char id_no = get_id_entrada(grafo);
        vector<char> fecho_transitivo_direto = grafo->fecho_transitivo_direto(id_no);

        string titulo = "FECHO TRANSITIVO DIRETO DE " + string(1, id_no);
        grafo->imprimirVetorVertices(fecho_transitivo_direto, titulo);

        if (pergunta_imprimir_arquivo("fecho_trans_dir.txt"))
        {
            grafo->salvarVetorVertices(fecho_transitivo_direto, "fecho_trans_dir.txt", titulo);
        }
        break;
    }

    case 'b':
    {

        char id_no = get_id_entrada(grafo);
        vector<char> fecho_transitivo_indireto = grafo->fecho_transitivo_indireto(id_no);

        string titulo = "FECHO TRANSITIVO INDIRETO DE " + string(1, id_no);
        grafo->imprimirVetorVertices(fecho_transitivo_indireto, titulo);

        if (pergunta_imprimir_arquivo("fecho_trans_indir.txt"))
        {
            grafo->salvarVetorVertices(fecho_transitivo_indireto, "fecho_trans_indir.txt", titulo);
        }
        break;
    }

    case 'c':
    {

        char id_no_1 = get_id_entrada(grafo);
        char id_no_2 = get_id_entrada(grafo);
        vector<char> caminho_minimo_dijkstra = grafo->caminho_minimo_dijkstra(id_no_1, id_no_2);

        string titulo = "CAMINHO MINIMO (DIJKSTRA) DE " + string(1, id_no_1) + " PARA " + string(1, id_no_2);
        grafo->imprimirCaminho(caminho_minimo_dijkstra, titulo);

        if (pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt"))
        {
            grafo->salvarCaminho(caminho_minimo_dijkstra, "caminho_minimo_dijkstra.txt", titulo);
        }
        break;
    }

    case 'd':
    {

        char id_no_1 = get_id_entrada(grafo);
        char id_no_2 = get_id_entrada(grafo);
        vector<char> caminho_minimo_floyd = grafo->caminho_minimo_floyd(id_no_1, id_no_2);
        string titulo = "CAMINHO MINIMO (FLOYD) DE " + string(1, id_no_1) + " PARA " + string(1, id_no_2);
        grafo->imprimirCaminho(caminho_minimo_floyd, titulo);

        if (pergunta_imprimir_arquivo("caminho_minimo_floyd.txt"))
        {
            grafo->salvarCaminho(caminho_minimo_floyd, "caminho_minimo_floyd.txt", titulo);
        }
        break;
    }

    case 'e':
    {

        int tam;
        cout << "Digite o tamanho do subconjunto: ";
        cin >> tam;

        if (cin.fail()) 
        {
            cout << "Entrada inválida!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }

        if (tam > 0 && tam <= grafo->ordem)
        {

            vector<char> ids = get_conjunto_ids(grafo, tam);
            Grafo *arvore_geradora_minima_prim = grafo->arvore_geradora_minima_prim(ids);

            if (arvore_geradora_minima_prim != nullptr)
            {
                cout << "ARVORE GERADORA MINIMA (PRIM)" << endl;
                arvore_geradora_minima_prim->imprimirGrafo();

                if (pergunta_imprimir_arquivo("agm_prim.txt"))
                {
                    arvore_geradora_minima_prim->salvarGrafo("agm_prim.txt");
                }

                delete arvore_geradora_minima_prim;
            }
        }
        else
        {
            cout << "Valor invalido" << endl;
            break;
        }
        break;
    }

    case 'f':
    {

        int tam;
        cout << "Digite o tamanho do subconjunto: ";
        cin >> tam;

        if (cin.fail()) {
            cout << "Entrada inválida!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }

        if (tam > 0 && tam <= grafo->ordem)
        {

            vector<char> ids = get_conjunto_ids(grafo, tam);
            Grafo *arvore_geradora_minima_kruskal = grafo->arvore_geradora_minima_kruskal(ids);
            if (arvore_geradora_minima_kruskal != nullptr)
            {
                cout << "ARVORE GERADORA MINIMA (KRUSKAL)" << endl;
                arvore_geradora_minima_kruskal->imprimirGrafo();

                if (pergunta_imprimir_arquivo("agm_kruskal.txt"))
                {
                    arvore_geradora_minima_kruskal->salvarGrafo("agm_kruskal.txt");
                }
                delete arvore_geradora_minima_kruskal;
            }
        }
        else
        {
            cout << "Valor invalido" << endl;
        }
        break;
    }

    case 'g':
    {

        char id_no = get_id_entrada(grafo);
        Grafo *arvore_caminhamento_profundidade = grafo->arvore_caminhamento_profundidade(id_no);

        if (arvore_caminhamento_profundidade != nullptr)
        {
            Gerenciador::imprimirResultadoDFS(arvore_caminhamento_profundidade);
            if (pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt"))
            {
                arvore_caminhamento_profundidade->salvarGrafo("arvore_caminhamento_profundidade.txt");
            }
            delete arvore_caminhamento_profundidade;
        }
        break;
    }

    case 'h':
    {
        int raio_grafo = grafo->raio();
        int diametro_grafo = grafo->diametro();
        vector<char> centro_grafo = grafo->centro();
        vector<char> periferia_grafo = grafo->periferia();
        grafo->imprimirPropriedades(raio_grafo, diametro_grafo, centro_grafo, periferia_grafo);

        if (pergunta_imprimir_arquivo("propriedades_grafo.txt"))
        {
            grafo->salvarPropriedades(raio_grafo, diametro_grafo, centro_grafo, periferia_grafo, "propriedades_grafo.txt");
        }
        break;
    }

    case 'i':
    {
        Gulosos* resolvedorCDS = new Gulosos(grafo);
        vector<char> conjuntoDominante = resolvedorCDS->ExecutarGulosoPuro();
        
        string titulo = "CONJUNTO DOMINANTE CONECTADO (GULOSO SIMPLES)";
        grafo->imprimirVetorVertices(conjuntoDominante, titulo);

        if (pergunta_imprimir_arquivo("cds_guloso_simples.txt")) {
            grafo->salvarVetorVertices(conjuntoDominante, "cds_guloso_simples.txt", titulo);
        }

        delete resolvedorCDS;
        break;
    }

    case 'j':
    {
        int numeroDeIteracoes, sementeDeRandomizacao;
        float fatorDeAleatoriedade;
        
        cout << "Digite o numero de iteracoes: ";
        cin >> numeroDeIteracoes;
        cout << "Digite o fator de aleatoriedade alfa inicial (ex: 0.3): ";
        cin >> fatorDeAleatoriedade;
        cout << "Digite a semente de randomizacao (um numero inteiro): ";
        cin >> sementeDeRandomizacao;

        Gulosos* resolvedorCDS = new Gulosos(grafo);
        
        vector<char> conjuntoDominanteConectado = resolvedorCDS->ExecutarGraParaDominanteConectado(
            numeroDeIteracoes, 
            fatorDeAleatoriedade, 
            sementeDeRandomizacao
        );
        
        string titulo = "CONJUNTO DOMINANTE CONECTADO (GRA)";
        grafo->imprimirVetorVertices(conjuntoDominanteConectado, titulo);

        if (pergunta_imprimir_arquivo("cds_gra.txt")) {
            grafo->salvarVetorVertices(conjuntoDominanteConectado, "cds_gra.txt", titulo);
        }

        delete resolvedorCDS;
        break;
    }

    case '0':
    {
        exit(0);
    }
    default:
    {
        cout << "Opção inválida" << endl;
        break;
    }
    }

    comandos(grafo);
}

char Gerenciador::get_id_entrada(Grafo *grafo)
{
    char id;
    while (true)
    {
        cout << "Digite o id de um no: ";
        cin >> id;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (grafo->getNo(id) != nullptr)
        {
            cout << endl;
            return id;
        }
        else
        {
            cout << "Erro: Vertice '" << id << "' nao encontrado. Tente novamente." << endl;
        }
    }
}

vector<char> Gerenciador::get_conjunto_ids(Grafo *grafo, int tam)
{
    vector<char> ids = {};
    while ((int)ids.size() < tam)
    {
        char id_no = get_id_entrada(grafo);
        bool existe = false;
        for (No *no : grafo->lista_adj)
        {
            if (no->id == id_no)
            {
                existe = true;
                break;
            }
        }

        if (!existe)
        {
            cout << "Vertice nao existe" << endl;
        }
        else
        {
            bool repetido = find(ids.begin(), ids.end(), id_no) != ids.end();
            if (repetido)
            {
                cout << "Valor repetido" << endl;
            }
            else
            {
                ids.push_back(id_no);
            }
        }
    }

    return ids;
}

bool Gerenciador::pergunta_imprimir_arquivo(string nome_arquivo)
{
    int resp;
    while (true)
    {

        cout << "Imprimir em arquivo externo? (" << nome_arquivo << ")" << endl;
        cout << "(1) Sim;" << endl;
        cout << "(2) Nao." << endl;
        cin >> resp;

        if (cin.fail())
        {
            cout << "Entrada invalida. Por favor, digite 1 ou 2" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (resp == 1)
        {
            cout << endl;
        }
        else if (resp == 2)
        {
            cout << endl;
            return false;
        }
        else
        {
            cout << "Resposta invalida. Digite 1 ou 2." << endl;
        }
    }
}