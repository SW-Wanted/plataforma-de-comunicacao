#include "../includes/platform.h"

#define DATA_PATH "data/membros.txt"

Membro* tabelaMembros[TAM_HASH] = { NULL };

int hash(const char *email) {
    int h = 0;
    while (*email)
        h += *email++;
    return h % TAM_HASH;
}

// Criacao de membro
Membro* criar_membro(const char *email, const char *senha, TipoMembro tipo) {
    Membro *m = malloc(sizeof(Membro));
    strcpy(m->email, email);
    strcpy(m->senha, senha);
    m->tipo = tipo;
    m->ativo = true;
    m->documentos = NULL;
    m->bloqueios = NULL;
    m->prox = NULL;
    return m;
}

void liberar_membro(Membro *m) {
    Documento *d = m->documentos;
    while (d) {
        Documento *tmp = d;
        d = d->prox;
        free(tmp);
    }

    Bloqueio *b = m->bloqueios;
    while (b) {
        Bloqueio *tmp = b;
        b = b->prox;
        free(tmp);
    }

    free(m);
}

// Conversor enum -> texto
const char* tipo_membro_str(TipoMembro tipo) {
    switch (tipo) {
        case ADMIN: return "Administrador";
        case CORPORATIVO: return "Corporativo";
        case CONVIDADO: return "Convidado";
        default: return "Desconhecido";
    }
}

// Impressao de perfil
void imprimir_perfil(const Membro *m) {
    printf("\n--- Perfil de Utilizador ---\n");
    printf("Email: %s\n", m->email);
    printf("Tipo: %s\n", tipo_membro_str(m->tipo));
    printf("Estado: %s\n", m->ativo ? "Ativo" : "Inativo");
    printf("Documentos:\n");
    Documento *d = m->documentos;
    while (d) {
        printf(" - %s\n", d->nome);
        d = d->prox;
    }
}

Membro* buscar_membro(const char *email) {
    if (email == NULL) return NULL;
    int h = hash(email);
    Membro *m = tabelaMembros[h];
    while (m) {
        if (strcmp(m->email, email) == 0) return m;
        m = m->prox;
    }
    return NULL;
}

// Insercao
void inserir_membro_hash(Membro *m) {
    int h = hash(m->email);
    m->prox = tabelaMembros[h];
    tabelaMembros[h] = m;
}

// Documentos
void adicionar_documento(Membro *m, const char *nomeDoc) {
    Documento *doc = malloc(sizeof(Documento));
    strcpy(doc->nome, nomeDoc);
    doc->prox = m->documentos;
    m->documentos = doc;
}

// Bloqueio
bool adicionar_bloqueio(Membro *origem, const char *emailDestino) {
    if (!origem || buscar_membro(emailDestino) == NULL || strcmp(origem->email, emailDestino) == 0)
        return false;

    Bloqueio *b = origem->bloqueios;
    while (b) {
        if (strcmp(b->emailBloqueado, emailDestino) == 0)
            return false; // ja bloqueado
        b = b->prox;
    }

    Bloqueio *novo = malloc(sizeof(Bloqueio));
    strcpy(novo->emailBloqueado, emailDestino);
    novo->prox = origem->bloqueios;
    origem->bloqueios = novo;
    return true;
}

bool esta_bloqueado(const Membro *origem, const char *emailDestino) {
    Bloqueio *b = origem->bloqueios;
    while (b) {
        if (strcmp(b->emailBloqueado, emailDestino) == 0)
            return true;
        b = b->prox;
    }
    return false;
}

// Persistencia
void salvar_membros() {
    FILE *f = fopen(DATA_PATH, "w");
    if (!f) return;

    int i; // declaração fora do for
    for (i = 0; i < TAM_HASH; i++) {
        Membro *m = tabelaMembros[i];
        while (m) {
            fprintf(f, "%s;%s;%d;%d\n", m->email, m->senha, m->tipo, m->ativo);
            Documento *d = m->documentos;
            while (d) {
                fprintf(f, "D;%s\n", d->nome);
                d = d->prox;
            }
            Bloqueio *b = m->bloqueios;
            while (b) {
                fprintf(f, "B;%s\n", b->emailBloqueado);
                b = b->prox;
            }
            m = m->prox;
        }
    }

    fclose(f);
}

void carregar_membros() {
    FILE *f = fopen(DATA_PATH, "r");
    if (!f) return;

    char linha[200];
    Membro *atual = NULL;

    while (fgets(linha, sizeof(linha), f)) {
        if (linha[0] == 'D') {
            Documento *d = malloc(sizeof(Documento));
            sscanf(linha, "D;%[^\n]", d->nome);
            d->prox = atual->documentos;
            atual->documentos = d;
        } else if (linha[0] == 'B') {
            Bloqueio *b = malloc(sizeof(Bloqueio));
            sscanf(linha, "B;%[^\n]", b->emailBloqueado);
            b->prox = atual->bloqueios;
            atual->bloqueios = b;
        } else {
            char email[EMAIL_MAX], senha[PASSWORD_MAX];
            int tipo, ativo;
            sscanf(linha, "%[^;];%[^;];%d;%d", email, senha, &tipo, &ativo);
            atual = criar_membro(email, senha, tipo);
            atual->ativo = ativo;
            inserir_membro_hash(atual);
        }
    }

    fclose(f);
}
