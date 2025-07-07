#include "../includes/hashtable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

HashTable *tabela_membros = NULL;

int funcao_hash(const char *email, int tamanho) {
    int hash = 0;
    while (*email)
        hash = (hash * 31 + *email++) % tamanho;
    return hash;
}

HashTable* criarHashTable(int tamanho) {
    HashTable *ht = (HashTable*) malloc(sizeof(HashTable));
    ht->tamanho = tamanho;
    ht->tabela = (NoHash**) malloc(tamanho * sizeof(NoHash*));
    int i;
    for (i = 0; i < tamanho; i++)
        ht->tabela[i] = NULL;

    return ht;
}

void inserir_membro(HashTable *ht, Membro *membro) {
    int idx = funcao_hash(membro->email, ht->tamanho);

    NoHash *novo = (NoHash*) malloc(sizeof(NoHash));
    novo->membro = membro;
    novo->prox = ht->tabela[idx];
    ht->tabela[idx] = novo;
}

Membro* buscar_membro_hash(HashTable *ht, const char *email) {
    int idx = funcao_hash(email, ht->tamanho);
    NoHash *atual = ht->tabela[idx];

    while (atual != NULL) {
        if (strcmp(atual->membro->email, email) == 0)
            return atual->membro;
        atual = atual->prox;
    }
    return NULL;
}

bool remover_membro(HashTable *ht, const char *email) {
    int idx = funcao_hash(email, ht->tamanho);
    NoHash *atual = ht->tabela[idx];
    NoHash *anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->membro->email, email) == 0) {
            if (anterior == NULL)
                ht->tabela[idx] = atual->prox;
            else
                anterior->prox = atual->prox;

            free(atual->membro); // Cuidado: s� se n�o for compartilhado!
            free(atual);
            return true;
        }
        anterior = atual;
        atual = atual->prox;
    }
    return false;
}

void imprimir_membros_hash(HashTable *ht) {
    int i;
    for (i = 0; i < ht->tamanho; i++) {
        printf("[%d]: ", i);
        NoHash *atual = ht->tabela[i];
        while (atual != NULL) {
            printf("%s -> ", atual->membro->email);
            atual = atual->prox;
        }
        printf("NULL\n");
    }
}

void liberarHashTable(HashTable *ht) {
    int i;
    for (i = 0; i < ht->tamanho; i++) {
        NoHash *atual = ht->tabela[i];
        while (atual != NULL) {
            NoHash *tmp = atual;
            atual = atual->prox;
            // Cuidado: liberar membro s� se n�o for usado em outro lugar
            free(tmp);
        }
    }
    free(ht->tabela);
    free(ht);
}
