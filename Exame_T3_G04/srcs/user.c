#include "../includes/platform.h"

Membro *tabelaMembros[TAM_HASH];

int hash(char *email) {
    int h = 0;
    int i;
    for (i = 0; email[i] != '\0'; i++)
        h += email[i];
    return h % TAM_HASH;
}

Membro* buscar_membro(char *email) {
    int h = hash(email);
    Membro *atual = tabelaMembros[h];
    while (atual) {
        if (strcmp(atual->email, email) == 0) return atual;
        atual = atual->prox;
    }
    return NULL;
}

void cadastrar_membro(char *email, char *senha, char tipo) {
    if (buscar_membro(email)) {
        printf("Membro j� existe!\n");
        return;
    }
    int h = hash(email);
    Membro *novo = malloc(sizeof(Membro));
    strcpy(novo->email, email);
    strcpy(novo->senha, senha);
    novo->tipo = tipo;
    novo->activo = 1;
    novo->documentos = NULL;
    novo->bloqueados = NULL;
    novo->prox = tabelaMembros[h];
    tabelaMembros[h] = novo;
    printf("Membro %s cadastrado com sucesso.\n", email);
}

int login(char *email, char *senha) {
    Membro *m = buscar_membro(email);
    if (m && m->activo && strcmp(m->senha, senha) == 0) {
        printf("\n Login bem-sucedido. Bem-vindo(a) %s.\n", email);
        return 1;
    }
    printf("\n Login falhou. Verifique o email/senha.\n");
    return 0;
}

void imprimir_perfil(char *email) {
    Membro *m = buscar_membro(email);
    if (!m) {
        printf("\n Membro n�o encontrado.\n");
        return;
    }
    printf("\n Perfil:\nEmail: %s\nTipo: %c\nEstado: %s\nDocumentos:\n", m->email, m->tipo, m->activo ? "Ativo" : "Inativo");
    Documento *doc = m->documentos;
    while (doc) {
        printf(" - %s\n", doc->nome);
        doc = doc->prox;
    }
}

void bloquear_membro(char *bloqueador, char *a_bloquear) {
    Membro *m1 = buscar_membro(bloqueador);
    Membro *m2 = buscar_membro(a_bloquear);
    if (!m1 || !m2) {
        printf("Um dos membros n�o existe.\n");
        return;
    }
    Membro *novo = malloc(sizeof(Membro));
    strcpy(novo->email, m2->email);
    novo->prox = m1->bloqueados;
    m1->bloqueados = novo;
    printf("Membro %s bloqueou %s.\n", bloqueador, a_bloquear);
}

int esta_bloqueado(char *origem, char *destino) {
    Membro *dest = buscar_membro(destino);
    if (!dest) return 0;
    Membro *b = dest->bloqueados;
    while (b) {
        if (strcmp(b->email, origem) == 0)
            return 1;
        b = b->prox;
    }
    return 0;
}

void guardar_membros() {
    FILE *f = fopen("membros.txt", "w");
    int i;
    for (i = 0; i < TAM_HASH; i++) {
        Membro *m = tabelaMembros[i];
        while (m) {
            fprintf(f, "%s;%s;%c;%d\n", m->email, m->senha, m->tipo, m->activo);
            Documento *d = m->documentos;
            while (d) {
                fprintf(f, "D;%s\n", d->nome);
                d = d->prox;
            }
            m = m->prox;
        }
    }
    fclose(f);
}

void carregar_membros() {
    FILE *f = fopen("data/membros/membros.txt", "r");
    if (!f) return;
    char linha[100];
    Membro *m = NULL;
    while (fgets(linha, sizeof(linha), f)) {
        if (linha[0] == 'D') {
            Documento *d = malloc(sizeof(Documento));
            sscanf(linha, "D;%[^\n]", d->nome);
            d->prox = m->documentos;
            m->documentos = d;
        } else {
            char email[50], senha[20], tipo;
            int activo;
            sscanf(linha, "%[^;];%[^;];%c;%d", email, senha, &tipo, &activo);
            cadastrar_membro(email, senha, tipo);
            m = buscar_membro(email);
            m->activo = activo;
        }
    }
    fclose(f);
}
