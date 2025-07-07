#ifndef TEAM_H
#define TEAM_H

#include "user.h"
#include <stdbool.h>

#define TEAM_NAME_MAX 50
#define TEAM_DATA_PATH "data/equipas.txt"

typedef struct TeamMember {
    Membro *membro;
    struct TeamMember *prox;
} TeamMember;

typedef struct Equipa {
    char nome[TEAM_NAME_MAX];
    bool publica;
    TeamMember *membros;
    struct Equipa *prox;
} Equipa;

Equipa* criar_equipa(const char *nome, bool publica);
Equipa* buscar_equipa(const Equipa *lista, const char *nome);

bool adicionar_membro_equipa(Equipa *e, Membro *m);
bool remover_membro_equipa(Equipa *e, const char *email);
bool membro_esta_na_equipa(const Equipa *e, const char *email);

void listar_equipas(const Equipa *lista);
void listar_equipas_publicas(const Equipa *lista);

void salvar_equipas();
Equipa *carregar_equipas();

#endif
