#ifndef EQUIPAS_H
#define EQUIPAS_H

#include "user.h"

typedef struct Equipa {
    char nome[50];
    int publica;
    Membro *membros;
    struct Equipa *prox;
} Equipa;

void criar_equipa(char *nome, int publica);
void adicionar_membro_equipa(char *email, char *nomeEquipa);
Equipa* buscar_equipa(char *nome);
void guardar_equipas();
void carregar_equipas();
void listar_equipas_publicas();
int juntar_a_equipa(char *email, char *nomeEquipa);
int membro_esta_na_equipa(char *email, Equipa *e);  

#endif
