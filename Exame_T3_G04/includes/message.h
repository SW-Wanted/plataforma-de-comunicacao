#ifndef MENSAGENS_H
#define MENSAGENS_H

#include <stdlib.h>

typedef struct Mensagem {
    char origem[50];
    char destino[50];
    char texto[200];
    struct Mensagem *prox;
} Mensagem;

void enviar_mensagem(char *origem, char *destino, char *texto);
void listar_mensagens();
void guardar_mensagens();
void carregar_mensagens();
void enviar_documento(char *email, char *nomeDoc);
extern Mensagem *listaMensagens;

#endif
