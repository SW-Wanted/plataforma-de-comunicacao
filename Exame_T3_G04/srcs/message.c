#include "../includes/platform.h"

Mensagem *mensagens = NULL;
Mensagem *listaMensagens = NULL;

void enviar_mensagem(char *origem, char *destino, char *texto) {
    Membro *rem = buscar_membro(origem);
    if (!rem || !rem->activo) {
        printf("Membro remetente invalido.\n");
        return;
    }

    if (rem->tipo == 'V') {
        printf("Membro convidado nao pode enviar mensagens.\n");
        return;
    }

    Equipa *eq = buscar_equipa(destino);
    if (!buscar_membro(destino) && !eq) {
        printf("Destinatario invalido.\n");
        return;
    }

    if (eq && eq->publica == 0) {
        Membro *m = eq->membros;
        int pertence = 0;
        while (m) {
            if (strcmp(m->email, origem) == 0) {
                pertence = 1;
                break;
            }
            m = m->prox;
        }
        if (!pertence) {
            printf("Acesso negado. A equipa '%s' � privada e voc� n�o faz parte dela.\n", eq->nome);
            return;
        }
    }

    if (esta_bloqueado(origem, destino)) {
        printf("O membro %s bloqueou %s. Mensagem n�o enviada.\n", destino, origem);
        return;
    }

    Mensagem *msg = malloc(sizeof(Mensagem));
    strcpy(msg->origem, origem);
    strcpy(msg->destino, destino);
    strcpy(msg->texto, texto);
    msg->prox = mensagens;
    mensagens = msg;
    printf("Mensagem enviada de %s para %s.\n", origem, destino);
}


void listar_mensagens() {
    Mensagem *msg = listaMensagens;
    while (msg) {
        Equipa *eq = buscar_equipa(msg->destino);
        if (eq) {
            if (!eq->publica && !membro_esta_na_equipa(utilizador_autenticado->email, eq)) {
                msg = msg->prox;
                continue;
            }
            printf("[Equipa: %s] %s ? %s\n", msg->destino, msg->origem, msg->texto);
        } else {
            if (strcmp(msg->origem, utilizador_autenticado->email) != 0 &&
                strcmp(msg->destino, utilizador_autenticado->email) != 0) {
                msg = msg->prox;
                continue;
            }
            printf("[Directa] %s ? %s: %s\n", msg->origem, msg->destino, msg->texto);
        }
        msg = msg->prox;
    }
}

void guardar_mensagens() {
    FILE *f = fopen("mensagens.txt", "w");
    Mensagem *msg = mensagens;
    while (msg) {
        fprintf(f, "%s;%s;%s\n", msg->origem, msg->destino, msg->texto);
        msg = msg->prox;
    }
    fclose(f);
}

void carregar_mensagens() {
    FILE *f = fopen("mensagens.txt", "r");
    if (!f) return;
    char linha[300];
    while (fgets(linha, sizeof(linha), f)) {
        char origem[50], destino[50], texto[200];
        sscanf(linha, "%[^;];%[^;];%[^\n]", origem, destino, texto);
        enviar_mensagem(origem, destino, texto);
    }
    fclose(f);
}

void enviar_documento(char *email, char *nomeDoc) {
    Membro *m = buscar_membro(email);
    if (!m) {
        printf("Membro n�o encontrado.\n");
        return;
    }

    if (m->tipo == 'V') {
        printf("Membro convidado n�o pode enviar documentos.\n");
        return;
    }

    Documento *novo = malloc(sizeof(Documento));
    strcpy(novo->nome, nomeDoc);
    novo->prox = m->documentos;
    m->documentos = novo;
    printf("Documento '%s' enviado e guardado no perfil de %s.\n", nomeDoc, email);
}
