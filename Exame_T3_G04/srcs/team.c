#include "../includes/platform.h"

Equipa *listaEquipas = NULL;

Equipa* buscar_equipa(char *nome) {
    Equipa *eq = listaEquipas;
    while (eq) {
        if (strcmp(eq->nome, nome) == 0) return eq;
        eq = eq->prox;
    }
    return NULL;
}

void criar_equipa(char *nome, int publica) {
    if (buscar_equipa(nome)) {
        printf("Equipa ja existe.\n");
        return;
    }
    Equipa *nova = malloc(sizeof(Equipa));
    strcpy(nova->nome, nome);
    nova->publica = publica;
    nova->membros = NULL;
    nova->prox = listaEquipas;
    listaEquipas = nova;
    printf("Equipa '%s' criada com sucesso.\n", nome);
}

void adicionar_membro_equipa(char *email, char *nomeEquipa) {
    Membro *m = buscar_membro(email);
    Equipa *e = buscar_equipa(nomeEquipa);
    if (!m || !e) {
        printf("Membro ou equipa n�o encontrado.\n");
        return;
    }
    m->prox = e->membros;
    e->membros = m;
    printf("Membro %s adicionado � equipa %s.\n", email, nomeEquipa);
}

void guardar_equipas() {
    FILE *f = fopen("equipas.txt", "w");
    Equipa *e = listaEquipas;
    while (e) {
        fprintf(f, "%s;%d\n", e->nome, e->publica);
        Membro *m = e->membros;
        while (m) {
            fprintf(f, "M;%s\n", m->email);
            m = m->prox;
        }
        e = e->prox;
    }
    fclose(f);
}

void carregar_equipas() {
    FILE *f = fopen("equipas.txt", "r");
    if (!f) return;
    char linha[100];
    Equipa *e = NULL;
    while (fgets(linha, sizeof(linha), f)) {
        if (linha[0] == 'M') {
            char email[50];
            sscanf(linha, "M;%[^\n]", email);
            adicionar_membro_equipa(email, e->nome);
        } else {
            char nome[50];
            int publica;
            sscanf(linha, "%[^;];%d", nome, &publica);
            criar_equipa(nome, publica);
            e = buscar_equipa(nome);
        }
    }
    fclose(f);
}

void listar_equipas() {
    Equipa *e = listaEquipas;
    if (!e) {
        printf("Nenhuma equipa criada ainda.\n");
        return;
    }
    while (e) {
        printf(" - %s [%s]\n", e->nome, e->publica ? "P�blica" : "Privada");
        e = e->prox;
    }
}

// equipas.c

void listar_equipas_publicas() {
    Equipa *e = listaEquipas;
    while (e) {
        if (e->publica) {
            printf("- %s\n", e->nome);
        }
        e = e->prox;
    }
}

int juntar_a_equipa(char *email, char *nomeEquipa) {
    Membro *m = buscar_membro(email);
    Equipa *e = buscar_equipa(nomeEquipa);
    if (!m || !e) return 0;
    if (!e->publica) return 0;
    // Verificar se ja esta na equipa
    Membro *membro_actual = e->membros;
    while (membro_actual) {
        if (strcmp(membro_actual->email, email) == 0) {
            return 0; // Ja esta na equipa
        }
        membro_actual = membro_actual->prox;
    }
    // Adicionar a equipa
    m->prox = e->membros;
    e->membros = m;
    return 1;
}

int membro_esta_na_equipa(char *email, Equipa *e) {
    Membro *m = e->membros;
    while (m) {
        if (strcmp(m->email, email) == 0) return 1;
        m = m->prox;
    }
    return 0;
}
