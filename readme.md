# DCC059 - Teoria dos Grafos - Turma A
## Trabalho Prático 2

### Membros do Grupo 18
- **Alexya Vitória de Jesus Silva** - 202265002AB - alexya.jesus@estudante.ufjf.br
- **Ana Luiza Almeida Míscoli** - 202265007AB - analuiza.miscoli@estudante.ufjf.br
- **Felipe Vasconcellos Strehle** - 202235028 - felipe.strehle@estudante.ufjf.br
- **Igor da Silva Amorim** - 202265140AB - 08867773640@estudante.ufjf.br
- **Kauê Oliveira Paraízo Garcia** - 202265517B - kaue.paraizo@estudante.ufjf.br

---

## 📋 Descrição do Projeto

Este projeto, desenvolvido para a disciplina de Teoria dos Grafos (DCC059), estende o **Tipo Abstrato de Dados (TAD)** de grafos do primeiro trabalho para incluir algoritmos construtivos focados na resolução do **Problema do Conjunto Dominante Conexo**.

O trabalho mantém todas as funcionalidades originais do TAD e adiciona uma nova classe com implementações de heurísticas gulosas para encontrar soluções para este problema de otimização.

### 🎯 Características do Grafo
- **Orientação**: Orientados ou não orientados
- **Ponderação**: Ponderados ou não ponderados (nos vértices e/ou nas arestas)
- **Representação**: Lista de adjacência

### 🔧 Funcionalidades Implementadas

**a)** **Fecho Transitivo Direto** - Parâmetro: ID de vértice (grafo direcionado)  
**b)** **Fecho Transitivo Indireto** - Parâmetro: ID de vértice (grafo direcionado)  
**c)** **Caminho Mínimo (Dijkstra)** - Parâmetro: dois IDs de vértices  
**d)** **Caminho Mínimo (Floyd)** - Parâmetro: dois IDs de vértices  
**e)** **Árvore Geradora Mínima (Prim)** - Parâmetro: subconjunto X de vértices  
**f)** **Árvore Geradora Mínima (Kruskal)** - Parâmetro: subconjunto X de vértices  
**g)** **Busca em Profundidade** - Parâmetro: ID de vértice  
**h)** **Propriedades do Grafo** - Raio, diâmetro, centro e periferia

**Novas Funcionalidades do Trabalho 2:**

**i)** **Algoritmo Guloso** para o Conjunto Dominante Conexo  
**j)** **Algoritmo Guloso Randomizado Adaptativo (GRASP)** para o Conjunto Dominante Conexo  
**k)** **Algoritmo Guloso Randomizado Adaptativo Reativo** para o Conjunto Dominante Conexo  

---

## 🚀 Como Executar

### Pré-requisitos
- Sistema Linux/macOS
- Compilador `g++`
- Arquivo de entrada com dados do grafo

### Instruções de Compilação

Para compilar o projeto, utilize o seguinte comando na pasta raiz:

```bash
cd src
g++ *.cpp -o execGrupo18
```

### Execução

Para executar o programa:

```bash
./execGrupo18 instancias/<arquivo_entrada>
```

**Exemplo:**
```bash
./execGrupo18 instancias/grafo_teste.txt
```

---

## 📁 Formato do Arquivo de Entrada

O arquivo de entrada deve seguir o seguinte formato:

```
0 1 0           # [direcionado] [ponderado_vertices] [ponderado_arestas]
5               # número de vértices
a               # ID do vértice 1
b               # ID do vértice 2
c               # ID do vértice 3
d               # ID do vértice 4
e               # ID do vértice 5
a b 2           # aresta: origem destino peso
b e 3           # (peso opcional se não ponderado)
e c 4
d e 1
a e 2
```

### Significado dos Parâmetros da Primeira Linha:
- **1º valor**: 1 se direcionado, 0 se não direcionado
- **2º valor**: 1 se ponderado nos vértices, 0 caso contrário
- **3º valor**: 1 se ponderado nas arestas, 0 caso contrário

---

## 🔍 Menu de Opções

O programa apresenta um menu interativo com as seguintes opções:

- **(a)** Fecho Transitivo Direto
- **(b)** Fecho Transitivo Indireto
- **(c)** Caminho Mínimo (Dijkstra)
- **(d)** Caminho Mínimo (Floyd)
- **(e)** Árvore Geradora Mínima (Prim)
- **(f)** Árvore Geradora Mínima (Kruskal)
- **(g)** Busca em Profundidade
- **(h)** Propriedades do Grafo
- **(i)** Algoritmo Guloso
- **(j)** Algoritmo Guloso Randomizado Adaptativo
- **(k)** Algoritmo Guloso Randomizado Adaptativo Reativo
- **(0)** Sair


Após cada execução, o programa pergunta se deseja salvar a saída em arquivo.

---

## 📧 Contato

Para dúvidas, entre em contato com qualquer um dos membros do grupo através dos emails listados acima.

---

## 📚 Referências

- **Disciplina**: DCC059 - Teoria dos Grafos
- **Professor**: Luciana Brugiolo
- **Tutor**: Rafael Freesz
- **Semestre**: 2025/1
- **Data de Entrega**: 11/08/2025
