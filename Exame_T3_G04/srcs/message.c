#include "../includes/message.h"

Mensagem *listaMensagens = NULL;

bool enviar_mensagem(const char *origem, const char *destino, const char *texto, Equipa *listaEquipas) {
    Membro *remetente = buscar_membro(origem);
    if (!remetente || !remetente->ativo || remetente->tipo == CONVIDADO) {
        printf("Erro: Remetente invalido ou sem permissoes.\n");
        return false;
    }

    // Verifica se destino e equipa ou membro
    Equipa *e = buscar_equipa(listaEquipas, destino);
    Membro *destinatario = buscar_membro(destino);

    if (!e && !destinatario) {
        printf("Erro: Destinatario invalido.\n");
        return false;
    }

    if (e) {
        if (!e->publica && !membro_esta_na_equipa(e, origem)) {
            printf("Erro: Acesso negado a equipa privada '%s'.\n", e->nome);
            return false;
        }
    }

    if (destinatario && esta_bloqueado(remetente, destino)) {
        printf("Erro: O membro %s bloqueou %s.\n", destino, origem);
        return false;
    }

    Mensagem *msg = malloc(sizeof(Mensagem));
    strcpy(msg->origem, origem);
    strcpy(msg->destino, destino);
    strcpy(msg->texto, texto);
    msg->prox = listaMensagens;
    listaMensagens = msg;

    printf("Mensagem enviada de %s para %s.\n", origem, destino);

    return true;
}

void listar_mensagens(Membro *utilizador, Equipa *listaEquipas) {
    Mensagem *msg = listaMensagens;
    while (msg) {
        Equipa *e = buscar_equipa(listaEquipas, msg->destino);

        if (e) {
            if (!e->publica && !membro_esta_na_equipa(e, utilizador->email)) {
                msg = msg->prox;
                continue;
            }
            printf("[Equipa: %s] %s -> Todos: %s\n", msg->destino, msg->origem, msg->texto);
        } else {
            if (strcmp(msg->origem, utilizador->email) != 0 && strcmp(msg->destino, utilizador->email) != 0) {
                msg = msg->prox;
                continue;
            }
            printf("[Directa] %s -> %s: %s\n", msg->origem, msg->destino, msg->texto);
        }
        msg = msg->prox;
    }
}

void guardar_mensagens() {
    FILE *f = fopen(MSG_FILE_PATH, "w");
    if (!f) return;

    Mensagem *msg = listaMensagens;
    while (msg) {
        fprintf(f, "%s;%s;%s\n", msg->origem, msg->destino, msg->texto);
        msg = msg->prox;
    }
    fclose(f);
}
void carregar_mensagens() {
    FILE *f = fopen(MSG_FILE_PATH, "r");
    if (!f) return;

    char linha[MSG_TAM_TEXTO + EMAIL_MAX * 2 + 10];
    while (fgets(linha, sizeof(linha), f)) {
        Mensagem *msg = malloc(sizeof(Mensagem));
        sscanf(linha, "%[^;];%[^;];%[^\n]", msg->origem, msg->destino, msg->texto);
        msg->prox = listaMensagens;
        listaMensagens = msg;
    }
    fclose(f);
}


bool enviar_documento(Membro *remetente, const char *nomeDoc) {
    if (!remetente || remetente->tipo == CONVIDADO) {
        printf("Erro: Permissao negada para envio de documentos.\n");
        return false;
    }

    Documento *d = malloc(sizeof(Documento));
    strcpy(d->nome, nomeDoc);
    d->prox = remetente->documentos;
    remetente->documentos = d;

    printf("Documento '%s' enviado e guardado no perfil de %s.\n", nomeDoc, remetente->email);
    return true;
}
