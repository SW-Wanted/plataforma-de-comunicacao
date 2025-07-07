#include "../includes/platform.h"

Membro *utilizador_autenticado = NULL;

void guardar_tudo() {
    guardar_membros();
    guardar_equipas();
    guardar_mensagens();
}

void carregar_tudo() {
    carregar_membros();
    carregar_equipas();
    carregar_mensagens();
}

int main() {
    setlocale(LC_ALL, "Portuguese");

    int op;
    char email[50], senha[20], tipo;
    char equipa[50], doc[50], destino[50], texto[200];
    Membro *utilizador_autenticado = NULL;

    carregar_tudo();

    while (!utilizador_autenticado) {
        printf("\n--- Plataforma de Comunicacao ---\n");
        printf("1. Cadastrar membro\n2. Iniciar sessao\n0. Sair\nOpcao: ");
        scanf("%d", &op);
        getchar();

        switch(op) {
            case 1:
                printf("Email: "); gets(email);
                printf("Palavra-passe: "); gets(senha);
                //printf("Tipo (A - Administrador, C - Corporativo, V - Convidado): "); scanf("%c", &tipo); getchar();
                cadastrar_membro(email, senha, 'V');
                break;
            case 2:
                printf("Email: "); gets(email);
                printf("Palavra-passe: "); gets(senha);
                Membro *m = buscar_membro(email);
                if (m && strcmp(m->senha, senha) == 0 && m->activo) {
                    utilizador_autenticado = m;
                    printf("\n Sessao iniciada como %s (%c).\n", m->email, m->tipo);
                } else {
                    printf("\n Credenciais invalidas ou conta inativa.\n");
                }
                break;
            case 0:
                guardar_tudo();
                return 0;
            default:
                printf("Opcao invalida.\n");
        }
    }

    while (1) {
        printf("\n--- Menu Principal ---\n");
        printf("1. Criar equipa\n2. Adicionar a equipa\n3. Enviar documento\n4. Imprimir perfil\n5. Enviar mensagem\n6. Ver mensagens\n7 Promover membros\n8. Juntar-se a equipa p�blica \n9. Terminar sess�o\n0. Sair\nOp��o: ");
        scanf("%d", &op);
        getchar();

        switch(op) {
            case 1:
                if (utilizador_autenticado->tipo != 'A') {
                    printf("Apenas administradores podem criar equipas.\n");
                    break;
                }
                printf("Nome da equipa: "); gets(equipa);
                printf("Publica? (1=Sim / 0=Nao): "); scanf("%d", &op); getchar();
                criar_equipa(equipa, op);
                break;
            case 2:
                if (utilizador_autenticado->tipo != 'A') {
                    printf("Apenas administradores podem adicionar membros a equipas.\n");
                    break;
                }
                printf("Email do membro: "); gets(email);
                printf("Equipas disponiveis:");
                listar_equipas();
                printf("Nome da equipa: "); gets(equipa);
                adicionar_membro_equipa(email, equipa);
                break;
            case 3:
            	if (utilizador_autenticado->tipo == 'V') {
            		printf("Membro convidado nao pode enviar documentos.\n");
                    break;
                }
                printf("Nome do documento: "); gets(doc);
                enviar_documento(utilizador_autenticado->email, doc);
                break;
            case 4:
                imprimir_perfil(utilizador_autenticado->email);
                break;
            case 5:
			    printf("Destino (email ou equipa): "); gets(destino);
			    Equipa *eq = buscar_equipa(destino);
			    if (eq) {
			        if (!membro_esta_na_equipa(utilizador_autenticado->email, eq)) {
			            printf("Nao pertence a equipa '%s'.\n", destino);
			            break;
			        }
			    }
			    printf("Mensagem: "); gets(texto);
			    enviar_mensagem(utilizador_autenticado->email, destino, texto);
			    break;
            case 6:
                listar_mensagens();
                break;
            case 7:
                if (utilizador_autenticado->tipo != 'A') {
                    printf("Apenas administradores podem promover membros.");
                    break;
                }
                printf("Email do membro a promover: "); gets(email);
                Membro *prom = buscar_membro(email);
                if (!prom) {
                    printf("Membro nao encontrado.");
                    break;
                }
                printf("Novo tipo (A - Administrador, C - Corporativo): "); scanf(" %c", &tipo); getchar();
                if (tipo == 'A' || tipo == 'C') {
                    prom->tipo = tipo;
                    printf("Membro promovido com sucesso.");
                } else {
                    printf("Tipo invalido. Apenas A ou C sao permitidos.");
                }
                break;
            case 8:
                if (utilizador_autenticado->tipo != 'C') {
                    printf("Apenas membros corporativos podem juntar-se a equipas publicas.\n");
                    break;
                }
                printf("Equipas publicas disponiveis:\n");
                listar_equipas_publicas();
                printf("Nome da equipa a juntar: "); gets(equipa);
                if (juntar_a_equipa(utilizador_autenticado->email, equipa)) {
                    printf("Juntou-se com sucesso a equipa '%s'.\n", equipa);
                } else {
                    printf("Erro ao tentar juntar-se a equipa.\n");
                }
                break;
            case 9:
                printf("Sessao terminada.\n");
                utilizador_autenticado = NULL;
                while (!utilizador_autenticado) {
                    printf("\n--- Plataforma de Comunicacao ---\n");
                    printf("1. Cadastrar membro\n2. Iniciar sessao\n0. Sair\nOpcao: ");
                    scanf("%d", &op);
                    getchar();
                    switch(op) {
                        case 1:
                            printf("Email: "); gets(email);
                            printf("Palavra-passe: "); gets(senha);
                            //printf("Tipo (A - Administrador, C - Corporativo, V - Convidado): "); scanf("%c", &tipo); getchar();
                            cadastrar_membro(email, senha, 'V');
                            break;
                        case 2:
                            printf("Email: "); gets(email);
                            printf("Palavra-passe: "); gets(senha);
                            Membro *m = buscar_membro(email);
                            if (m && strcmp(m->senha, senha) == 0 && m->activo) {
                                utilizador_autenticado = m;
                                printf("\n Sessao iniciada como %s (%c).\n", m->email, m->tipo);
                            } else {
                                printf("\n Credenciais invalidas ou conta inativa.\n");
                            }
                            break;
                        case 0:
                            guardar_tudo();
                            return 0;
                        default:
                            printf("Opcao invalida.\n");
                    }
                }
                break;
            case 0:
                guardar_tudo();
                return 0;
            default:
                printf("Opcao invalida.\n");
        }
    }
}
