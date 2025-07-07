#include "../includes/platform.h"

Membro *utilizador_logado = NULL;
Equipa *todas_equipas = NULL;

void guardar_tudo() {
    salvar_membros();
    salvar_equipas();
    guardar_mensagens();
}

void carregar_tudo() {
    carregar_membros();
    todas_equipas = carregar_equipas();
    carregar_mensagens();
}

// Sessao
bool cadastrar_utilizador(const char *nome, const char *email, const char *senha, char tipo) {
    if (buscar_membro(email)) return false;
    Membro *novo = criar_membro(email, senha, tipo);
    inserir_membro_hash(novo); // de hashtable
    return true;
}

bool login(const char *email, const char *senha) {
    Membro *m = buscar_membro(email);
    if (m && m->ativo && strcmp(m->senha, senha) == 0) {
        utilizador_logado = m;
        return true;
    }
    return false;
}

void logout() {
    utilizador_logado = NULL;
}

// Perfil
void ver_perfil() {
    if (utilizador_logado)
        imprimir_perfil(utilizador_logado);
}

bool promover_utilizador(const char *email, char novo_tipo) {
    Membro *m = buscar_membro(email);
    if (!m || !utilizador_logado || utilizador_logado->tipo != ADMIN) return false;
    m->tipo = (novo_tipo == 'A') ? ADMIN : (novo_tipo == 'C' ? CORPORATIVO : m->tipo);
    return true;
}

bool bloquear_utilizador(const char *alvo_email) {
    return adicionar_bloqueio(utilizador_logado, alvo_email);
}

// Equipa
bool criar_nova_equipa(const char *nome, bool publica) {
    if (!utilizador_logado || utilizador_logado->tipo != ADMIN) return false;
    Equipa *nova = criar_equipa(nome, publica);
    if (!nova) return false;
    nova->prox = todas_equipas;
    todas_equipas = nova;
    return true;
}

bool adicionar_membro_para_equipa(const char *email, const char *nome_equipa) {
    if (!utilizador_logado || utilizador_logado->tipo != ADMIN) return false;

    Equipa *e = buscar_equipa(todas_equipas, nome_equipa);
    Membro *m = buscar_membro(email);
    if (!e || !m) return false;
    return adicionar_membro_equipa(e, m);
}

bool juntar_a_equipa_publica(const char *nome_equipa) {
    if (!utilizador_logado || utilizador_logado->tipo != CORPORATIVO) return false;
    Equipa *e = buscar_equipa(todas_equipas, nome_equipa);
    if (!e || !e->publica) return false;
    return adicionar_membro_equipa(e, utilizador_logado);
}

void listar_equipas_ativas() {
    listar_equipas(todas_equipas);
}

void listar_todas_equipas_publicas() {
    listar_equipas_publicas(todas_equipas);
}

// Mensagens e documentos
bool enviar_msg(const char *destino, const char *mensagem) {
    return enviar_mensagem(utilizador_logado->email, destino, mensagem, todas_equipas);
}

bool enviar_doc(const char *nomeDoc) {
    return enviar_documento(utilizador_logado, nomeDoc);
}

void ver_mensagens() {
    listar_mensagens(utilizador_logado, todas_equipas);
}

void guardar_sistema() {
    salvar_membros();
    salvar_equipas();
    guardar_mensagens();
}
