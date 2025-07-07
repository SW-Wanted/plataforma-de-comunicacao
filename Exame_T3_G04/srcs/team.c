#include "../includes/platform.h"

// Adicione ou ajuste o define para o caminho correto
#define TEAM_DATA_PATH "data/equipas.txt"

// Lista global de equipas
static Equipa *listaEquipas = NULL;

// Cria uma nova equipa
Equipa* criar_equipa(const char *nome, bool publica) {
    Equipa *e = malloc(sizeof(Equipa));
    if (!e) return NULL;
    strncpy(e->nome, nome, TEAM_NAME_MAX-1);
    e->nome[TEAM_NAME_MAX-1] = '\0';
    e->publica = publica;
    e->membros = NULL;
    e->prox = NULL;
    return e;
}

// Busca equipa na lista
Equipa* buscar_equipa(const Equipa *lista, const char *nome) {
    while (lista) {
        if (strcmp(lista->nome, nome) == 0)
            return (Equipa *)lista;
        lista = lista->prox;
    }
    return NULL;
}

// Verifica se membro esta na equipa
bool membro_esta_na_equipa(const Equipa *e, const char *email) {
    TeamMember *t = e->membros;
    while (t) {
        if (strcmp(t->membro->email, email) == 0)
            return true;
        t = t->prox;
    }
    return false;
}

// Adiciona membro a equipa
bool adicionar_membro_equipa(Equipa *e, Membro *m) {
    if (!e || !m) return false;
    if (membro_esta_na_equipa(e, m->email)) return false;
    TeamMember *novo = malloc(sizeof(TeamMember));
    if (!novo) return false;
    novo->membro = m;
    novo->prox = e->membros;
    e->membros = novo;
    return true;
}

// Remove membro da equipa
bool remover_membro_equipa(Equipa *e, const char *email) {
    TeamMember *atual = e->membros, *anterior = NULL;
    while (atual) {
        if (strcmp(atual->membro->email, email) == 0) {
            if (anterior) anterior->prox = atual->prox;
            else e->membros = atual->prox;
            free(atual);
            return true;
        }
        anterior = atual;
        atual = atual->prox;
    }
    return false;
}

// Lista todas as equipas
void listar_equipas(const Equipa *lista) {
    while (lista) {
        printf("- %s [%s]\n", lista->nome, lista->publica ? "Publica" : "Privada");
        lista = lista->prox;
    }
}

// Lista apenas equipas publicas
void listar_equipas_publicas(const Equipa *lista) {
    while (lista) {
        if (lista->publica)
            printf("- %s\n", lista->nome);
        lista = lista->prox;
    }
}

// Salva equipas e membros em arquivo
void salvar_equipas() {
    FILE *f = fopen(TEAM_DATA_PATH, "w");
    if (!f) return;
    Equipa *lista = listaEquipas;
    while (lista) {
        fprintf(f, "%s;%d\n", lista->nome, lista->publica);
        TeamMember *tm = lista->membros;
        while (tm) {
            fprintf(f, "M;%s\n", tm->membro->email);
            tm = tm->prox;
        }
        lista = lista->prox;
    }
    fclose(f);
}

// Carrega equipas com callback de busca de membro
Equipa* carregar_equipas() {
    FILE *f = fopen(TEAM_DATA_PATH, "r");
    if (!f) return NULL;

    Equipa *lista = NULL, *ultima = NULL;
    char linha[200];
    while (fgets(linha, sizeof(linha), f)) {
        char nome[TEAM_NAME_MAX];
        int publica;
        if (sscanf(linha, "%[^;];%d", nome, &publica) != 2) continue;

        Equipa *e = criar_equipa(nome, publica);
        if (!e) continue;

        if (ultima) ultima->prox = e;
        else lista = e;
        ultima = e;

        while (fgets(linha, sizeof(linha), f) && linha[0] == 'M') {
            char email[EMAIL_MAX];
            if (sscanf(linha + 2, "%s", email) != 1) continue;
            Membro *membro = buscar_membro(email);
            if (membro && adicionar_membro_equipa(e, membro)) {
                printf("Membro %s adicionado a equipa %s.\n", email, nome);
            }
        }
    }
    fclose(f);
    return lista;
}
