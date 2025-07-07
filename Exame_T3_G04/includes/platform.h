#ifndef PLATFORM_H
#define PLATFORM_H

#include "user.h"
#include "team.h"
#include "message.h"
#include "hashtable.h"
#include "graph.h"

// Sessao ativa
extern Membro *utilizador_logado;
extern Equipa *todas_equipas;

void guardar_tudo();
void carregar_tudo();
void guardar_sistema();

// Acoes do sistema
bool cadastrar_utilizador(const char *nome, const char *email, const char *senha, char tipo);
bool login(const char *email, const char *senha);
void logout();

// Perfil e gestao
void ver_perfil();
bool promover_utilizador(const char *email, char tipo);
bool bloquear_utilizador(const char *alvo_email);

// Equipa
bool criar_nova_equipa(const char *nome, bool publica);
bool adicionar_membro_para_equipa(const char *email, const char *nome_equipa);
bool juntar_a_equipa_publica(const char *nome_equipa);
void listar_todas_equipas_publicas();

// Mensagens e documentos
bool enviar_msg(const char *destino, const char *mensagem);
bool enviar_doc(const char *nomeDoc);
void ver_mensagens();

#endif
