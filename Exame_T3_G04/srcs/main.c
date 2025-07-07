#include "../includes/platform.h"
#include <locale.h>

// Declaracao externa da tabela de membros
extern Membro* tabelaMembros[];

// Prototipagem dos submenus
void menu_admin();
void menu_corporativo();
void menu_convidado();
void menu_autenticacao();

// Função simples para validar email (deve conter '@' e '.')
int validar_email(const char *email) {
    const char *at = strchr(email, '@');
    const char *dot = strrchr(email, '.');
    return at && dot && at < dot && strlen(email) >= 5;
}

// Função simples para validar senha (mínimo 4 caracteres)
int validar_senha(const char *senha) {
    return senha && strlen(senha) >= 4;
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    carregar_tudo(); // Carrega dados do sistema

    while (1) {
        // Se nao ha utilizador logado, mostrar menu de autenticacao
        if (!utilizador_logado) {
            menu_autenticacao();
        } else {
            switch (utilizador_logado->tipo) {
                case ADMIN: menu_admin(); break;
                case CORPORATIVO: menu_corporativo(); break;
                case CONVIDADO: menu_convidado(); break;
            }
        }
    }

    guardar_sistema(); // (opcional se quiser salvar no fim)
    return 0;
}

// ---------------- MENU AUTENTICACAO ----------------
void menu_autenticacao() {
    int op;
    char nome[50], email[50], senha[50];

    // Cores ANSI
    #define RESET   "\033[0m"
    #define BOLD    "\033[1m"
    #define CYAN    "\033[36m"
    #define YELLOW  "\033[33m"
    #define GREEN   "\033[32m"
    #define RED     "\033[31m"
    #define WHITE   "\033[97m"

    // Verifica se existe algum ADMIN cadastrado
    int existe_admin = 0;
    int i;
    for (i = 0; i < TAM_HASH; i++) {
        Membro *m = tabelaMembros[i];
        while (m) {
            if (m->tipo == ADMIN) {
                existe_admin = 1;
                break;
            }
            m = m->prox;
        }
        if (existe_admin) break;
    }

    printf(CYAN BOLD "\n    +--------------------------------------+\n");
    printf("    |" RESET WHITE "      PLATAFORMA DE COMUNICACAO       " CYAN "|\n");
    printf("    +--------------------------------------+\n" RESET);
    printf("    |" YELLOW " 1 " RESET "| " WHITE "Cadastrar-se                     " CYAN "|\n");
    printf("    |" YELLOW " 2 " RESET "| " WHITE "Iniciar Sessao                   " CYAN "|\n");
    printf("    |" YELLOW " 0 " RESET "| " WHITE "Sair                             " CYAN "|\n");
    printf("    +--------------------------------------+\n" RESET);
    printf(BOLD "    Escolha: " RESET);
    scanf("%d", &op);
    getchar();

    switch (op) {
        case 1:
            printf(GREEN "\n    +------------------- Cadastro -------------------+\n" RESET);
            printf("    Nome: "); fgets(nome, sizeof(nome), stdin); strtok(nome, "\n");
            do {
                printf("    Email: "); fgets(email, sizeof(email), stdin); strtok(email, "\n");
                if (!validar_email(email))
                    printf(RED "    [!] Email invalido. Tente novamente.\n" RESET);
            } while (!validar_email(email));
            do {
                printf("    Senha (min 4 chars): "); fgets(senha, sizeof(senha), stdin); strtok(senha, "\n");
                if (!validar_senha(senha))
                    printf(RED "    [!] Senha invalida. Tente novamente.\n" RESET);
            } while (!validar_senha(senha));
            if (!existe_admin) {
                if (!buscar_membro(email)) {
                    printf(YELLOW "    [!] Este sera o ADMINISTRADOR do sistema.\n" RESET);
                }
                if (cadastrar_utilizador(nome, email, senha, ADMIN)) {
                    printf(GREEN "    [✓] Cadastro concluido como ADMINISTRADOR.\n" RESET);
                    salvar_membros();
                } else {
                    printf(RED "    [!] Email ja cadastrado.\n" RESET);
                }
            } else {
                if (cadastrar_utilizador(nome, email, senha, CONVIDADO)) {
                    printf(GREEN "    [✓] Cadastro concluido como CONVIDADO.\n" RESET);
                    salvar_membros();
                } else {
                    printf(RED "    [!] Email ja cadastrado.\n" RESET);
                }
            }
            printf(CYAN "    +------------------------------------------------+\n" RESET);
            break;
        case 2:
            printf(GREEN "\n    +-------------------- Login ---------------------+\n" RESET);
            printf("    Email: "); fgets(email, sizeof(email), stdin); strtok(email, "\n");
            printf("    Senha: "); fgets(senha, sizeof(senha), stdin); strtok(senha, "\n");
            if (login(email, senha)) {
                printf(GREEN "    [✓] Login bem-sucedido! Bem-vindo %s.\n" RESET, email);
            } else {
                printf(RED "    [!] Credenciais invalidas.\n" RESET);
            }
            printf(CYAN "    +------------------------------------------------+\n" RESET);
            break;
        case 0:
            printf(YELLOW "\n    +------------------- Encerrando -----------------+\n" RESET);
            guardar_sistema();
            printf(GREEN "    [✓] Dados salvos. Ate logo!\n" RESET);
            printf(CYAN "    +------------------------------------------------+\n" RESET);
            exit(0);
        default:
            printf(RED "    [!] Opcao invalida.\n" RESET);
    }

    // Undefine macros to avoid pollution
    #undef RESET
    #undef BOLD
    #undef CYAN
    #undef YELLOW
    #undef GREEN
    #undef RED
    #undef WHITE
}

// ---------------- MENU ADMINISTRADOR ----------------
void menu_admin() {
    int op;
    char nome[50], email[50], senha[50], tipo, equipa[50];
    int publica;
    char destino[50], texto[200], doc[50];

    // Cores ANSI
    #define RESET   "\033[0m"
    #define BOLD    "\033[1m"
    #define CYAN    "\033[36m"
    #define YELLOW  "\033[33m"
    #define GREEN   "\033[32m"
    #define RED     "\033[31m"
    #define WHITE   "\033[97m"

    printf(CYAN BOLD "\n    +---------------------- MENU ADMINISTRADOR ----------------------+\n" RESET);
    printf("    |" YELLOW " 1 " RESET "| " WHITE "Criar nova equipa                                          " CYAN "|\n");
    printf("    |" YELLOW " 2 " RESET "| " WHITE "Adicionar membro a equipa                                  " CYAN "|\n");
    printf("    |" YELLOW " 3 " RESET "| " WHITE "Remover membro da equipa                                   " CYAN "|\n");
    printf("    |" YELLOW " 4 " RESET "| " WHITE "Desativar membro                                           " CYAN "|\n");
    printf("    |" YELLOW " 5 " RESET "| " WHITE "Promover membro                                            " CYAN "|\n");
    printf("    |" YELLOW " 6 " RESET "| " WHITE "Ver perfil                                                 " CYAN "|\n");
    printf("    |" YELLOW " 7 " RESET "| " WHITE "Ver mensagens                                              " CYAN "|\n");
    printf("    |" YELLOW " 8 " RESET "| " WHITE "Listar equipas                                             " CYAN "|\n");
    printf("    |" YELLOW " 9 " RESET "| " WHITE "Listar equipas publicas                                    " CYAN "|\n");
    printf("    |" YELLOW "10 " RESET "| " WHITE "Bloquear utilizador                                        " CYAN "|\n");
    printf("    |" YELLOW "99" RESET "| " WHITE "Terminar sessao                                             " CYAN "|\n");
    printf("    |" YELLOW " 0 " RESET "| " WHITE "Sair                                                       " CYAN "|\n");
    printf("    +----------------------------------------------------------------+\n" RESET);
    printf(BOLD "    Escolha: " RESET);
    scanf("%d", &op); getchar();

    switch (op) {
        case 1:
            printf(CYAN "\n    +------ Criar Nova Equipa ------+\n" RESET);
            printf("    Nome da equipa: "); fgets(equipa, sizeof(equipa), stdin); strtok(equipa, "\n");
            printf("    E publica? (1=Sim, 0=Nao): "); scanf("%d", &publica); getchar();
            if (criar_nova_equipa(equipa, publica ? true : false))
                printf(GREEN "    [✓] Equipa criada com sucesso.\n" RESET);
            else
                printf(RED "    [!] Erro ao criar equipa.\n" RESET);
            printf(CYAN "    +--------------------------------+\n" RESET);
            break;
        case 2:
            printf(CYAN "\n    +------ Adicionar Membro a Equipa ------+\n" RESET);
            printf("    Email do membro: "); fgets(email, sizeof(email), stdin); strtok(email, "\n");
            printf("    Nome da equipa: "); fgets(equipa, sizeof(equipa), stdin); strtok(equipa, "\n");
            if (adicionar_membro_para_equipa(email, equipa))
                printf(GREEN "    [✓] Membro adicionado.\n" RESET);
            else
                printf(RED "    [!] Erro ao adicionar membro.\n" RESET);
            printf(CYAN "    +---------------------------------------+\n" RESET);
            break;
        case 3:
            printf(CYAN "\n    +------ Remover Membro da Equipa ------+\n" RESET);
            printf("    Email do membro: "); fgets(email, sizeof(email), stdin); strtok(email, "\n");
            printf("    Nome da equipa: "); fgets(equipa, sizeof(equipa), stdin); strtok(equipa, "\n");
            if (remover_membro_equipa(buscar_equipa(todas_equipas, equipa), email))
                printf(GREEN "    [✓] Membro removido da equipa.\n" RESET);
            else
                printf(RED "    [!] Erro ao remover membro.\n" RESET);
            printf(CYAN "    +--------------------------------------+\n" RESET);
            break;
        case 4:
            printf(CYAN "\n    +------ Desativar Membro ------+\n" RESET);
            printf("    Email do membro a desativar: "); fgets(email, sizeof(email), stdin); strtok(email, "\n");
            {
                Membro *m = buscar_membro(email);
                if (m && m != utilizador_logado) {
                    m->ativo = false;
                    printf(GREEN "    [✓] Membro desativado.\n" RESET);
                } else {
                    printf(RED "    [!] Erro ao desativar membro.\n" RESET);
                }
            }
            printf(CYAN "    +------------------------------+\n" RESET);
            break;
        case 5:
            printf(CYAN "\n    +------ Promover Membro ------+\n" RESET);
            printf("    Email do membro a promover: "); fgets(email, sizeof(email), stdin); strtok(email, "\n");
            printf("    Novo tipo (A, C): "); scanf(" %c", &tipo); getchar();
            if (promover_utilizador(email, tipo))
                printf(GREEN "    [✓] Promocao concluida.\n" RESET);
            else
                printf(RED "    [!] Erro na promocao.\n" RESET);
            printf(CYAN "    +-----------------------------+\n" RESET);
            break;
        case 6:
            printf(CYAN "\n    +------ Perfil ------+\n" RESET);
            ver_perfil();
            printf(CYAN "    +--------------------+\n" RESET);
            break;
        case 7:
            printf(CYAN "\n    +------ Mensagens ------+\n" RESET);
            ver_mensagens();
            printf(CYAN "    +-----------------------+\n" RESET);
            break;
        case 8:
            printf(CYAN "\n    +------ Equipas Ativas ------+\n" RESET);
            listar_equipas_ativas();
            printf(CYAN "    +----------------------------+\n" RESET);
            break;
        case 9:
            printf(CYAN "\n    +------ Equipas Publicas ------+\n" RESET);
            listar_equipas_publicas(todas_equipas);
            printf(CYAN "    +------------------------------+\n" RESET);
            break;
        case 10:
            printf(CYAN "\n    +------ Bloquear Utilizador ------+\n" RESET);
            printf("    Email do utilizador a bloquear: "); fgets(destino, sizeof(destino), stdin); strtok(destino, "\n");
            if (bloquear_utilizador(destino))
                printf(GREEN "    [✓] Utilizador bloqueado.\n" RESET);
            else
                printf(RED "    [!] Erro ao bloquear utilizador.\n" RESET);
            printf(CYAN "    +----------------------------------+\n" RESET);
            break;
        case 99:
            printf(YELLOW "\n    +------ Terminando Sessao ------+\n" RESET);
            logout();
            printf(GREEN "    [✓] Sessao terminada.\n" RESET);
            printf(CYAN "    +-------------------------------+\n" RESET);
            break;
        case 0:
            printf(YELLOW "\n    +------ Encerrando ------+\n" RESET);
            guardar_sistema();
            printf(GREEN "    [✓] Dados salvos. Ate logo!\n" RESET);
            printf(CYAN "    +------------------------+\n" RESET);
            exit(0);
        default:
            printf(RED "    [!] Opcao invalida.\n" RESET);
    }

    #undef RESET
    #undef BOLD
    #undef CYAN
    #undef YELLOW
    #undef GREEN
    #undef RED
    #undef WHITE
}

// ---------------- MENU CORPORATIVO ----------------
void menu_corporativo() {
    int op;
    char destino[50], texto[200], doc[50], equipa[50];

    // Cores ANSI
    #define RESET   "\033[0m"
    #define BOLD    "\033[1m"
    #define CYAN    "\033[36m"
    #define YELLOW  "\033[33m"
    #define GREEN   "\033[32m"
    #define RED     "\033[31m"
    #define WHITE   "\033[97m"

    printf(CYAN BOLD "\n    +-------------------------------+\n");
    printf("    |" RESET WHITE "      MENU CORPORATIVO         " CYAN "|\n");
    printf("    +-------------------------------+\n" RESET);
    printf("    |" YELLOW " 1 " RESET "| " WHITE "Enviar mensagem           " CYAN "|\n");
    printf("    |" YELLOW " 2 " RESET "| " WHITE "Enviar documento          " CYAN "|\n");
    printf("    |" YELLOW " 3 " RESET "| " WHITE "Ver mensagens             " CYAN "|\n");
    printf("    |" YELLOW " 4 " RESET "| " WHITE "Juntar-se a equipa publica" CYAN "|\n");
    printf("    |" YELLOW " 5 " RESET "| " WHITE "Ver perfil                " CYAN "|\n");
    printf("    |" YELLOW " 6 " RESET "| " WHITE "Listar equipas publicas   " CYAN "|\n");
    printf("    |" YELLOW " 7 " RESET "| " WHITE "Bloquear utilizador        " CYAN "|\n");
    printf("    |" YELLOW " 9 " RESET "| " WHITE "Terminar sessao           " CYAN "|\n");
    printf("    |" YELLOW " 0 " RESET "| " WHITE "Sair                      " CYAN "|\n");
    printf("    +-------------------------------+\n" RESET);
    printf(BOLD "    Escolha: " RESET);
    scanf("%d", &op); getchar();

    switch (op) {
        case 1:
            printf("Destino: "); fgets(destino, sizeof(destino), stdin); strtok(destino, "\n");
            printf("Mensagem: "); fgets(texto, sizeof(texto), stdin); strtok(texto, "\n");
            enviar_msg(destino, texto);
            break;
        case 2:
            printf("Nome do documento: "); fgets(doc, sizeof(doc), stdin); strtok(doc, "\n");
            enviar_doc(doc);
            break;
        case 3:
            ver_mensagens();
            break;
        case 4:
            printf("Nome da equipa publica: "); fgets(equipa, sizeof(equipa), stdin); strtok(equipa, "\n");
            if (juntar_a_equipa_publica(equipa))
                printf("Entrou na equipa com sucesso.\n");
            else
                printf("Erro ao entrar na equipa.\n");
            break;
        case 5:
            ver_perfil();
            break;
        case 6:
            listar_equipas_publicas(todas_equipas);
            break;
        case 7:
            printf("Email do utilizador a bloquear: "); fgets(destino, sizeof(destino), stdin); strtok(destino, "\n");
            bloquear_utilizador(destino);
            break;
        case 9:
            logout();
            break;
        case 0:
            guardar_sistema();
            exit(0);
        default:
            printf("Opcao invalida.\n");
    }

    // Undefine macros to evitar poluicao
    #undef RESET
    #undef BOLD
    #undef CYAN
    #undef YELLOW
    #undef GREEN
    #undef RED
    #undef WHITE
}

// ---------------- MENU CONVIDADO ----------------
void menu_convidado() {
    int op;

    // Cores ANSI
    #define RESET   "\033[0m"
    #define BOLD    "\033[1m"
    #define CYAN    "\033[36m"
    #define YELLOW  "\033[33m"
    #define GREEN   "\033[32m"
    #define RED     "\033[31m"
    #define WHITE   "\033[97m"

    printf(CYAN BOLD "\n    +-------------------------------+\n");
    printf("    |" RESET WHITE "        MENU CONVIDADO         " CYAN "|\n");
    printf("    +-------------------------------+\n" RESET);
    printf("    |" YELLOW " 1 " RESET "| " WHITE "Ver mensagens             " CYAN "|\n");
    printf("    |" YELLOW " 2 " RESET "| " WHITE "Ver perfil                " CYAN "|\n");
    printf("    |" YELLOW " 9 " RESET "| " WHITE "Terminar sessao           " CYAN "|\n");
    printf("    |" YELLOW " 0 " RESET "| " WHITE "Sair                      " CYAN "|\n");
    printf("    +-------------------------------+\n" RESET);
    printf(BOLD "    Escolha: " RESET);
    scanf("%d", &op); getchar();

    switch (op) {
        case 1:
            ver_mensagens();
            break;
        case 2:
            ver_perfil();
            break;
        case 9:
            logout();
            break;
        case 0:
            guardar_sistema();
            exit(0);
        default:
            printf("Opcao invalida.\n");
    }

    // Undefine macros to evitar poluicao
    #undef RESET
    #undef BOLD
    #undef CYAN
    #undef YELLOW
    #undef GREEN
    #undef RED
    #undef WHITE
}
