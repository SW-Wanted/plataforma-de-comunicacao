#ifndef USER_H
#define USER_H

#include <stdbool.h>

#define EMAIL_MAX    100
#define PASSWORD_MAX 50
#define DATA_PATH    "data/membros.txt"

typedef enum {
    ADMIN,
    CORPORATIVO,
    CONVIDADO
} TipoMembro;

// Documento em lista ligada
typedef struct Documento {
    char nome[50];
    struct Documento *prox;
} Documento;

// Lista de bloqueios
typedef struct Bloqueio {
    char emailBloqueado[EMAIL_MAX];
    struct Bloqueio *prox;
} Bloqueio;

// Estrutura de um Membro
typedef struct Membro {
    char email[EMAIL_MAX];
    char senha[PASSWORD_MAX];
    TipoMembro tipo;
    bool ativo;
    Documento *documentos;      // documentos pessoais
    Bloqueio *bloqueios;  // emails bloqueados
    struct Membro *prox;  // para colis�es na tabela hash
} Membro;

// Fun��es principais
Membro* criar_membro(const char *email, const char *senha, TipoMembro tipo);
void liberar_membro(Membro *m);
void adicionar_documento(Membro *m, const char *nomeDoc);
Membro* buscar_membro(const char *email);
int hash(const char *email);
void inserir_membro_hash(Membro *m);
void imprimir_perfil(const Membro *m);
bool adicionar_bloqueio(Membro *origem, const char *emailDestino);
bool esta_bloqueado(const Membro *origem, const char *emailDestino);

// Persistência
void salvar_membros();
void carregar_membros();

#endif
