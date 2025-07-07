#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>
#include "user.h"

#define TAM_HASH 50

typedef struct noHash {
    Membro *membro;
    struct noHash *prox;
} NoHash;

typedef struct hashTable {
    int tamanho;
    NoHash **tabela;
} HashTable;

// Funções principais
HashTable* criarHashTable(int tamanho);
void inserir_membro(HashTable *ht, Membro *membro);
Membro* buscar_membro_hash(HashTable *ht, const char *email);
bool remover_membro(HashTable *ht, const char *email);
void imprimir_membros_hash(HashTable *ht);
void liberarHashTable(HashTable *ht);

// Função de hashing com strings
int funcao_hash(const char *email, int tamanho);

extern HashTable *tabela_membros;

#endif

