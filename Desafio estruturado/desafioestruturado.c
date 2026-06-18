// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//
// Versão modularizada com sistema de missões, alocação dinâmica de memória,
// passagem por referência (ponteiros) e por referência constante (const).
//
// ============================================================================

// --- Inclusão das bibliotecas padrão ---
#include <stdio.h>   // Entrada/saída (printf, scanf, fgets, getchar)
#include <stdlib.h>  // Alocação dinâmica (calloc, free), rand, srand
#include <string.h>  // Manipulação de strings (strcpy, strcmp, strcspn)
#include <time.h>    // time() para a semente do gerador aleatório
#include <locale.h>  // setlocale para acentuação em português

// --- Constantes Globais ---
#define NUM_TERRITORIOS 5    // Quantidade de territórios no mapa
#define NUM_MISSOES     5    // Quantidade de missões possíveis
#define TAM_STRING      30   // Tamanho máximo das strings (nome, cor)

// Cor controlada pelo jogador neste jogo.
#define COR_JOGADOR "Azul"

// --- Estrutura de Dados ---
// Representa um território do mapa: nome, cor do exército dominante e tropas.
typedef struct {
    char nome[TAM_STRING];
    char cor[TAM_STRING];
    int  tropas;
} Territorio;

// --- Protótipos das Funções ---

// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa(void);
void inicializarTerritorios(Territorio *mapa);
void liberarMemoria(Territorio *mapa);

// Funções de interface com o usuário:
void exibirMenuPrincipal(void);
void exibirMapa(const Territorio *mapa);
void exibirMissao(int idMissao);

// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio *mapa);
void simularAtaque(Territorio *mapa, int origem, int destino);
int  sortearMissao(void);
int  verificarVitoria(const Territorio *mapa, int idMissao);

// Função utilitária:
void limparBufferEntrada(void);

// ============================================================================
// --- Função Principal (main) ---
// ============================================================================
int main(void) {
    // 1. Configuração Inicial (Setup)
    setlocale(LC_ALL, "Portuguese");     // Locale para português
    srand((unsigned int) time(NULL));    // Semente para números aleatórios

    // Aloca a memória para o mapa e verifica se deu certo.
    Territorio *mapa = alocarMapa();
    if (mapa == NULL) {
        printf("Erro: falha ao alocar memoria para o mapa.\n");
        return 1;
    }

    // Preenche os territórios com os dados iniciais.
    inicializarTerritorios(mapa);

    // Sorteia a missão secreta do jogador.
    int idMissao = sortearMissao();

    int opcao;
    int venceu = 0;

    // 2. Laço Principal do Jogo (Game Loop)
    do {
        printf("\n############################################\n");
        printf("#            WAR ESTRUTURADO               #\n");
        printf("############################################\n");
        printf("Voce comanda o exercito: %s\n", COR_JOGADOR);

        exibirMapa(mapa);
        exibirMissao(idMissao);
        exibirMenuPrincipal();

        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            // Entrada inválida: limpa o buffer e recomeça.
            limparBufferEntrada();
            opcao = -1;
            continue;
        }
        limparBufferEntrada();

        switch (opcao) {
            case 1: // Atacar
                faseDeAtaque(mapa);
                break;

            case 2: // Verificar missão
                if (verificarVitoria(mapa, idMissao)) {
                    printf("\n>>> PARABENS! Voce cumpriu sua missao e VENCEU o jogo! <<<\n");
                    venceu = 1;
                } else {
                    printf("\nMissao ainda nao concluida. Continue lutando!\n");
                }
                break;

            case 0: // Sair
                printf("\nEncerrando o jogo. Ate a proxima!\n");
                break;

            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                break;
        }

        // Pausa para o jogador ler os resultados antes da próxima rodada.
        if (opcao != 0 && !venceu) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }

    } while (opcao != 0 && !venceu);

    // 3. Limpeza
    liberarMemoria(mapa);

    return 0;
}

// ============================================================================
// --- Implementação das Funções ---
// ============================================================================

// alocarMapa():
// Aloca dinamicamente o vetor de territórios usando calloc (zera a memória).
// Retorna o ponteiro alocado ou NULL em caso de falha.
Territorio* alocarMapa(void) {
    return (Territorio*) calloc(NUM_TERRITORIOS, sizeof(Territorio));
}

// inicializarTerritorios():
// Preenche os dados iniciais de cada território. Modifica o mapa por referência.
void inicializarTerritorios(Territorio *mapa) {
    strcpy(mapa[0].nome, "Brasil");    strcpy(mapa[0].cor, "Azul");     mapa[0].tropas = 5;
    strcpy(mapa[1].nome, "Argentina"); strcpy(mapa[1].cor, "Verde");    mapa[1].tropas = 3;
    strcpy(mapa[2].nome, "Egito");     strcpy(mapa[2].cor, "Vermelho"); mapa[2].tropas = 4;
    strcpy(mapa[3].nome, "Franca");    strcpy(mapa[3].cor, "Amarelo");  mapa[3].tropas = 2;
    strcpy(mapa[4].nome, "Japao");     strcpy(mapa[4].cor, "Verde");    mapa[4].tropas = 6;
}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa.
void liberarMemoria(Territorio *mapa) {
    free(mapa);
}

// exibirMenuPrincipal():
// Imprime o menu de ações disponíveis.
void exibirMenuPrincipal(void) {
    printf("\n--- MENU DE ACOES ---\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar missao\n");
    printf("0 - Sair\n");
}

// exibirMapa():
// Mostra o estado atual de todos os territórios, formatado como tabela.
// 'const' garante que a função apenas lê os dados, sem alterá-los.
void exibirMapa(const Territorio *mapa) {
    printf("\n--- ESTADO ATUAL DO MAPA ---\n");
    printf("%-3s %-12s %-12s %-7s\n", "Nro", "Territorio", "Cor", "Tropas");
    printf("------------------------------------------\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%-3d %-12s %-12s %-7d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("------------------------------------------\n");
}

// exibirMissao():
// Exibe a descrição da missão atual com base no ID sorteado.
void exibirMissao(int idMissao) {
    printf("\n--- SUA MISSAO SECRETA ---\n");
    switch (idMissao) {
        case 0:
            printf("Destruir o exercito VERDE (elimine todos os territorios verdes).\n");
            break;
        case 1:
            printf("Conquistar pelo menos 3 territorios.\n");
            break;
        case 2:
            printf("Destruir o exercito VERMELHO (elimine todos os territorios vermelhos).\n");
            break;
        case 3:
            printf("Conquistar pelo menos 4 territorios.\n");
            break;
        case 4:
            printf("Destruir o exercito AMARELO (elimine todos os territorios amarelos).\n");
            break;
        default:
            printf("Missao desconhecida.\n");
            break;
    }
}

// faseDeAtaque():
// Interface da ação de ataque. Solicita origem e destino e chama simularAtaque().
void faseDeAtaque(Territorio *mapa) {
    int origem, destino;

    printf("\n--- FASE DE ATAQUE ---\n");
    printf("Numero do territorio de ORIGEM (atacante): ");
    if (scanf("%d", &origem) != 1) { limparBufferEntrada(); printf("Entrada invalida.\n"); return; }
    printf("Numero do territorio de DESTINO (defensor): ");
    if (scanf("%d", &destino) != 1) { limparBufferEntrada(); printf("Entrada invalida.\n"); return; }
    limparBufferEntrada();

    // Converte de 1-based (usuário) para 0-based (vetor).
    simularAtaque(mapa, origem - 1, destino - 1);
}

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Faz validações, rola os dados, compara e atualiza as tropas.
// Em caso de conquista, troca o dono do território e move uma tropa.
void simularAtaque(Territorio *mapa, int origem, int destino) {
    // Validação de índices.
    if (origem < 0 || origem >= NUM_TERRITORIOS ||
        destino < 0 || destino >= NUM_TERRITORIOS) {
        printf("\nTerritorio invalido!\n");
        return;
    }

    // Não se pode atacar a si mesmo.
    if (origem == destino) {
        printf("\nA origem e o destino nao podem ser o mesmo territorio.\n");
        return;
    }

    // A origem precisa pertencer ao jogador.
    if (strcmp(mapa[origem].cor, COR_JOGADOR) != 0) {
        printf("\nVoce so pode atacar a partir de um territorio seu (%s).\n", COR_JOGADOR);
        return;
    }

    // Não se ataca um território da própria cor.
    if (strcmp(mapa[destino].cor, COR_JOGADOR) == 0) {
        printf("\nVoce nao pode atacar um territorio do seu proprio exercito.\n");
        return;
    }

    // É preciso ter mais de 1 tropa para atacar (uma fica defendendo).
    if (mapa[origem].tropas <= 1) {
        printf("\nTropas insuficientes em %s para atacar (precisa de mais de 1).\n",
               mapa[origem].nome);
        return;
    }

    // Rolagem dos dados (1 a 6).
    int dadoAtaque = rand() % 6 + 1;
    int dadoDefesa = rand() % 6 + 1;

    printf("\n%s ataca %s!\n", mapa[origem].nome, mapa[destino].nome);
    printf("Dado do atacante: %d | Dado do defensor: %d\n", dadoAtaque, dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        // Defensor perde uma tropa.
        mapa[destino].tropas--;
        printf("O atacante venceu a rodada! %s perde 1 tropa.\n", mapa[destino].nome);

        // Conquista: defensor zerado.
        if (mapa[destino].tropas <= 0) {
            printf(">>> %s foi CONQUISTADO por %s! <<<\n",
                   mapa[destino].nome, mapa[origem].nome);
            strcpy(mapa[destino].cor, mapa[origem].cor); // muda o dono
            mapa[destino].tropas = 1;                     // move 1 tropa
            mapa[origem].tropas--;                        // tropa sai da origem
        }
    } else {
        // Empate ou defesa maior: atacante perde uma tropa.
        mapa[origem].tropas--;
        printf("O defensor resistiu! %s perde 1 tropa.\n", mapa[origem].nome);
    }
}

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório.
int sortearMissao(void) {
    return rand() % NUM_MISSOES;
}

// verificarVitoria():
// Verifica se o jogador cumpriu a missão. Retorna 1 (cumprida) ou 0 (não).
// 'const' porque apenas lê o mapa.
int verificarVitoria(const Territorio *mapa, int idMissao) {
    switch (idMissao) {
        case 0: // Destruir exército VERDE
            for (int i = 0; i < NUM_TERRITORIOS; i++)
                if (strcmp(mapa[i].cor, "Verde") == 0) return 0;
            return 1;

        case 2: // Destruir exército VERMELHO
            for (int i = 0; i < NUM_TERRITORIOS; i++)
                if (strcmp(mapa[i].cor, "Vermelho") == 0) return 0;
            return 1;

        case 4: // Destruir exército AMARELO
            for (int i = 0; i < NUM_TERRITORIOS; i++)
                if (strcmp(mapa[i].cor, "Amarelo") == 0) return 0;
            return 1;

        case 1: { // Conquistar pelo menos 3 territórios
            int qtd = 0;
            for (int i = 0; i < NUM_TERRITORIOS; i++)
                if (strcmp(mapa[i].cor, COR_JOGADOR) == 0) qtd++;
            return (qtd >= 3) ? 1 : 0;
        }

        case 3: { // Conquistar pelo menos 4 territórios
            int qtd = 0;
            for (int i = 0; i < NUM_TERRITORIOS; i++)
                if (strcmp(mapa[i].cor, COR_JOGADOR) == 0) qtd++;
            return (qtd >= 4) ? 1 : 0;
        }

        default:
            return 0;
    }
}

// limparBufferEntrada():
// Descarta o que sobrou no buffer de entrada (stdin) até o fim da linha.
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* descarta */ }
}