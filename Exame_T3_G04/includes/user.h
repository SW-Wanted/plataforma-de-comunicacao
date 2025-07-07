#ifndef MEMBROS_H
#define MEMBROS_H

#define EMAIL_MAX 100
#define NAME_MAX 50
#define PASSWORD_MAX 50
#define DOCS_MAX 100
#define TAM_HASH 50


typedef struct Documento {
    char nome[50];
    struct Documento *prox;
} Documento;

typedef struct Membro {
    char email[50];
    char senha[20];
    char tipo; // 'A', 'C', 'V'
    int activo;
    Documento *documentos;
    struct Membro *bloqueados;
    struct Membro *prox;
} Membro;

void cadastrar_membro(char *email, char *senha, char tipo);
Membro* buscar_membro(char *email);
int login(char *email, char *senha);
void imprimir_perfil(char *email);
void bloquear_membro(char *bloqueador, char *a_bloquear);
int esta_bloqueado(char *origem, char *destino);
void guardar_membros();
void carregar_membros();
extern Membro *utilizador_autenticado;

#endif
