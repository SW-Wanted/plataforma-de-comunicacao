#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "user.h"
#include "team.h"

#define MSG_TAM_TEXTO 200
#define MSG_FILE_PATH "data/mensagens.txt"

typedef struct Mensagem {
    char origem[EMAIL_MAX];
    char destino[EMAIL_MAX]; // Pode ser email ou nome da equipa
    char texto[MSG_TAM_TEXTO];
    struct Mensagem *prox;
} Mensagem;

// Lista de todas as mensagens carregadas
extern Mensagem *listaMensagens;

// Funções principais
bool enviar_mensagem(const char *origem, const char *destino, const char *texto, Equipa *listaEquipas);
void listar_mensagens(Membro *utilizador, Equipa *listaEquipas);

// Persistência
void guardar_mensagens();
void carregar_mensagens();

// Documentos
bool enviar_documento(Membro *remetente, const char *nomeDoc);

#endif
