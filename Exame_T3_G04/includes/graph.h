#ifndef GRAPH_H
#define GRAPH_H

#include "user.h"

typedef struct Vizinho {
    Membro *destino;
    struct Vizinho *prox;
} Vizinho;

typedef struct Vertice {
    Membro *origem;
    Vizinho *adj;
    struct Vertice *prox;
} Vertice;

typedef struct Grafo {
    Vertice *vertices;
} Grafo;

// Funções principais
Grafo* criar_grafo();
void adicionar_aresta(Grafo *g, Membro *origem, Membro *destino);
void mostrar_grafo(Grafo *g);
Vertice* buscar_vertice(Grafo *g, Membro *origem);
void liberar_grafo(Grafo *g);

#endif
