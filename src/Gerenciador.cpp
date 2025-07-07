#include "Gerenciador.h"
#include <fstream>


void Gerenciador::imprimirResultadoDFS(Grafo* arvore) {
    cout << "ARVORE DE CAMINHAMENTO EM PROFUNDIDADE" << endl;
    arvore->imprimirGrafo(); // imprime a estrutura principal da árvore usando a função genérica 
    cout << "ARESTAS DE RETORNO (CICLOS):" << endl; // adiciona as arestas de retorno
    if (arvore->arestas_de_retorno.empty()) {
        cout << "Nenhuma" << endl;
    } else {
        for (const auto& aresta : arvore->arestas_de_retorno) {
            cout << "  " << aresta.first << " -> " << aresta.second << endl;
        }
    }
    cout << "=========================================" << endl << endl;
}

void Gerenciador::comandos(Grafo* grafo) {
    cout<<"Digite uma das opcoes abaixo e pressione enter:"<<endl<<endl;
    cout<<"(a) Fecho transitivo direto de um no;"<<endl;
    cout<<"(b) Fecho transitivo indireto de um no;"<<endl;
    cout<<"(c) Caminho minimo (Djikstra);"<<endl;
    cout<<"(d) Caminho minimo (Floyd);"<<endl;
    cout<<"(e) Arvore Geradora Minima (Algoritmo de Prim);"<<endl;
    cout<<"(f) Arvore Geradora Minima (Algoritmo de Kruskal);"<<endl;
    cout<<"(g) Arvore de caminhamento em profundidade;"<<endl;
    cout<<"(h) Raio, diametro, centro e periferia do grafo;"<<endl;
    cout<<"(0) Sair;"<<endl<<endl;

    char resp;
    cin >> resp;
    switch (resp) {
        case 'a': {

            char id_no = get_id_entrada();
            vector<char> fecho_transitivo_direto = grafo->fecho_transitivo_direto(id_no);
            
            string titulo = "FECHO TRANSITIVO DIRETO DE " + string(1, id_no);
            grafo->imprimirVetorVertices(fecho_transitivo_direto, titulo);

            if(pergunta_imprimir_arquivo("fecho_trans_dir.txt")) {
                grafo->salvarVetorVertices(fecho_transitivo_direto, "fecho_trans_dir.txt", titulo);
            }
            break;
        }

        case 'b':{

            char id_no = get_id_entrada();
            vector<char> fecho_transitivo_indireto = grafo->fecho_transitivo_indireto(id_no);
            
            string titulo = "FECHO TRANSITIVO INDIRETO DE " + string(1, id_no);
            grafo->imprimirVetorVertices(fecho_transitivo_indireto, titulo);

            if(pergunta_imprimir_arquivo("fecho_trans_indir.txt")) {
                grafo->salvarVetorVertices(fecho_transitivo_indireto, "fecho_trans_indir.txt", titulo);
            }
            break;
        }

        case 'c': {

            char id_no_1 = get_id_entrada();
            char id_no_2 = get_id_entrada();
            vector<char> caminho_minimo_dijkstra = grafo->caminho_minimo_dijkstra(id_no_1,id_no_2);

            string titulo = "CAMINHO MINIMO (DIJKSTRA) DE " + string(1, id_no_1) + " PARA " + string(1, id_no_2);
            grafo->imprimirCaminho(caminho_minimo_dijkstra, titulo);

            if(pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt")) {
                grafo->salvarCaminho(caminho_minimo_dijkstra, "caminho_minimo_dijkstra.txt", titulo);
            }
            break;
        }

        case 'd': {

            char id_no_1 = get_id_entrada();
            char id_no_2 = get_id_entrada();
            vector<char> caminho_minimo_floyd = grafo->caminho_minimo_floyd(id_no_1,id_no_2);
            string titulo = "CAMINHO MINIMO (FLOYD) DE " + string(1, id_no_1) + " PARA " + string(1, id_no_2);
            grafo->imprimirCaminho(caminho_minimo_floyd, titulo);

            if(pergunta_imprimir_arquivo("caminho_minimo_floyd.txt")) {
                grafo->salvarCaminho(caminho_minimo_floyd, "caminho_minimo_floyd.txt", titulo);
            }
            break;
        }

        case 'e': {

            int tam;
            cout<<"Digite o tamanho do subconjunto: ";
            cin>>tam;

            if(tam > 0 && tam <= grafo->ordem) {

                vector<char> ids = get_conjunto_ids(grafo,tam);
                Grafo* arvore_geradora_minima_prim = grafo->arvore_geradora_minima_prim(ids);

                if(arvore_geradora_minima_prim != nullptr) {
                    cout << "ARVORE GERADORA MINIMA (PRIM)" << endl;
                    arvore_geradora_minima_prim->imprimirGrafo();
        
                if(pergunta_imprimir_arquivo("agm_prim.txt")) {
                    arvore_geradora_minima_prim->salvarGrafo("agm_prim.txt");
                }
        
                delete arvore_geradora_minima_prim;
            }
            } else {
                cout<<"Valor invalido"<<endl;
            }
            break;
        }

        case 'f': {

            int tam;
            cout<<"Digite o tamanho do subconjunto: ";
            cin>>tam;

            if(tam > 0 && tam <= grafo->ordem) {

                vector<char> ids = get_conjunto_ids(grafo,tam);
                Grafo* arvore_geradora_minima_kruskal = grafo->arvore_geradora_minima_kruskal(ids);
                if(arvore_geradora_minima_kruskal != nullptr) {
                    cout << "ARVORE GERADORA MINIMA (KRUSKAL)" << endl;
                    arvore_geradora_minima_kruskal->imprimirGrafo();

                    if(pergunta_imprimir_arquivo("agm_kruskal.txt")) {
                        arvore_geradora_minima_kruskal->salvarGrafo("agm_kruskal.txt");
                    }
                    delete arvore_geradora_minima_kruskal;
                }
            } else {
                cout<<"Valor invalido"<<endl;
            }
            break;
        }

        case 'g': {

            char id_no = get_id_entrada();
            Grafo* arvore_caminhamento_profundidade = grafo->arvore_caminhamento_profundidade(id_no);

            if(arvore_caminhamento_profundidade != nullptr) {
                Gerenciador::imprimirResultadoDFS(arvore_caminhamento_profundidade);
                if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                    arvore_caminhamento_profundidade->salvarGrafo("arvore_caminhamento_profundidade.txt");
                }
                delete arvore_caminhamento_profundidade;
            }
            break;
        }

        case 'h': {

        case '0': {
            exit(0);
        }
        default: {
            cout<<"Opção inválida"<<endl;
        }
    }

    comandos(grafo);
}

char Gerenciador::get_id_entrada() {
    cout<<"Digite o id de um no: ";
    char id;
    cin>>id;
    cout<<endl;
    return id;
}

vector<char> Gerenciador::get_conjunto_ids(Grafo *grafo, int tam) {
    vector<char> ids = {};
    while((int)ids.size() < tam) {
        char id_no =get_id_entrada();
        bool existe = false;
        for(No* no: grafo->lista_adj){
            if(no->id == id_no){
                existe = true;
                break;
            }
        }

        if(!existe){
            cout<<"Vertice nao existe"<<endl;
        }else{
            bool repetido = find(ids.begin(), ids.end(),id_no) != ids.end();
            if(repetido) {
                    cout<<"Valor repetido"<<endl;
            }else {
                ids.push_back(id_no);
            }
        }

    }

    return ids;
}


bool Gerenciador::pergunta_imprimir_arquivo(string nome_arquivo) {

    cout<<"Imprimir em arquivo externo? ("<<nome_arquivo<<")"<<endl;
    cout<<"(1) Sim;"<<endl;
    cout<<"(2) Nao."<<endl;
    int resp;
    cin>>resp;
    cout<<endl;

    switch (resp) {
        case 1:
            return true;
        case 2:
            return false;
        default:
            cout<<"Resposta invalida"<<endl;
            return pergunta_imprimir_arquivo(nome_arquivo);
    }
}