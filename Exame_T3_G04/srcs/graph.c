#include "../includes/graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Grafo* criar_grafo() {
    Grafo *g = (Grafo*) malloc(sizeof(Grafo));
    g->vertices = NULL;
    return g;
}

Vertice* buscar_vertice(Grafo *g, Membro *origem) {
    Vertice *v = g->vertices;
    while (v != NULL) {
        if (strcmp(v->origem->email, origem->email) == 0)
            return v;
        v = v->prox;
    }
    return NULL;
}

void adicionar_vizinho(Vizinho **lista, Membro *destino) {
    Vizinho *novo = (Vizinho*) malloc(sizeof(Vizinho));
    novo->destino = destino;
    novo->prox = *lista;
    *lista = novo;
}

void adicionar_aresta(Grafo *g, Membro *origem, Membro *destino) {
    // Origem
    Vertice *vOrigem = buscar_vertice(g, origem);
    if (!vOrigem) {
        vOrigem = (Vertice*) malloc(sizeof(Vertice));
        vOrigem->origem = origem;
        vOrigem->adj = NULL;
        vOrigem->prox = g->vertices;
        g->vertices = vOrigem;
    }
    adicionar_vizinho(&(vOrigem->adj), destino);

    // Destino (não-direcionado)
    Vertice *vDestino = buscar_vertice(g, destino);
    if (!vDestino) {
        vDestino = (Vertice*) malloc(sizeof(Vertice));
        vDestino->origem = destino;
        vDestino->adj = NULL;
        vDestino->prox = g->vertices;
        g->vertices = vDestino;
    }
    adicionar_vizinho(&(vDestino->adj), origem);
}

void mostrar_grafo(Grafo *g) {
    Vertice *v = g->vertices;
    while (v != NULL) {
        printf("%s:", v->origem->email);
        Vizinho *adj = v->adj;
        while (adj != NULL) {
            printf(" -> %s", adj->destino->email);
            adj = adj->prox;
        }
        printf(" -> NULL\n");
        v = v->prox;
    }
}

void liberar_grafo(Grafo *g) {
    Vertice *v = g->vertices;
    while (v != NULL) {
        Vizinho *adj = v->adj;
        while (adj != NULL) {
            Vizinho *tmpAdj = adj;
            adj = adj->prox;
            free(tmpAdj);
        }
        Vertice *tmp = v;
        v = v->prox;
        free(tmp);
    }
    free(g);
}
