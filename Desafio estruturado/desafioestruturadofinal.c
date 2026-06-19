/* ============================================================================
 *  WAR ESTRUTURADO - NIVEL MESTRE
 *  Sistema de Missoes Estrategicas Individuais
 * ----------------------------------------------------------------------------
 *  Este programa simula uma versao simplificada do jogo War, onde cada jogador
 *  recebe, no inicio do jogo, uma missao sorteada automaticamente. A missao e
 *  armazenada dinamicamente (malloc) e verificada silenciosamente ao final de
 *  cada turno. Quando a condicao de vitoria de algum jogador e atingida, o
 *  vencedor e declarado.
 *
 *  Conceitos praticados: modularizacao, ponteiros, passagem por valor e por
 *  referencia, alocacao dinamica (malloc/calloc) e liberacao de memoria (free).
 * ==========================================================================*/

#include <stdio.h>   // Entrada e saida padrao (printf)
#include <stdlib.h>  // Alocacao dinamica (malloc, calloc, free) e rand/srand
#include <string.h>  // Manipulacao de strings (strcpy, strcmp, strstr)
#include <time.h>    // Funcao time() usada como semente do gerador aleatorio

/* ---------- Constantes do jogo ---------- */
#define TAM_MAPA     5    // Quantidade de territorios no mapa
#define TOTAL_MISSOES 5   // Quantidade de missoes disponiveis para sorteio
#define TAM_MISSAO   100  // Tamanho maximo (em caracteres) de uma missao
#define MAX_TURNOS   20   // Limite de turnos da simulacao

/* ---------- Estrutura que representa um territorio ---------- */
typedef struct {
    char nome[30];  // Nome do territorio
    char cor[10];   // Cor do exercito que domina o territorio
    int  tropas;    // Quantidade de tropas presentes no territorio
} Territorio;

/* ---------- Prototipos das funcoes (modularizacao) ---------- */
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
void exibirMissao(char* missao);
int  verificarMissao(char* missao, Territorio* mapa, int tamanho);
void atacar(Territorio* atacante, Territorio* defensor);
void exibirMapa(Territorio* mapa, int tamanho);
void liberarMemoria(Territorio* mapa, char* missaoJogador1, char* missaoJogador2);
int  contarTerritoriosPorCor(Territorio* mapa, int tamanho, const char* cor);

/* ============================================================================
 *  atribuirMissao
 *  Sorteia uma missao do vetor de missoes e a copia (via strcpy) para o
 *  endereco apontado por 'destino'. A missao do jogador e passada POR
 *  REFERENCIA (ponteiro), pois o conteudo apontado e modificado aqui.
 * ==========================================================================*/
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;   // sorteia um indice valido
    strcpy(destino, missoes[indice]);     // copia a missao sorteada para o jogador
}

/* ============================================================================
 *  exibirMissao
 *  Exibe a missao do jogador. A missao e passada POR VALOR (apenas leitura):
 *  a funcao usa o conteudo da string, mas nao o altera.
 * ==========================================================================*/
void exibirMissao(char* missao) {
    printf("  >> Sua missao: %s\n", missao);
}

/* ============================================================================
 *  contarTerritoriosPorCor
 *  Funcao auxiliar: conta quantos territorios pertencem a uma determinada cor.
 * ==========================================================================*/
int contarTerritoriosPorCor(Territorio* mapa, int tamanho, const char* cor) {
    int cont = 0;
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, cor) == 0) {
            cont++;
        }
    }
    return cont;
}

/* ============================================================================
 *  verificarMissao
 *  Verifica se a missao do jogador foi cumprida com base no estado atual do
 *  mapa. A missao e passada POR REFERENCIA (ponteiro) e o mapa tambem.
 *  Retorna 1 se a missao foi cumprida e 0 caso contrario.
 *
 *  Logica simples inicial: identifica o tipo da missao pelo texto (strstr) e
 *  avalia a condicao correspondente. Pode ser facilmente expandida para
 *  considerar a cor especifica de cada jogador.
 * ==========================================================================*/
int verificarMissao(char* missao, Territorio* mapa, int tamanho) {

    /* Missao de eliminacao do exercito Verde (cor neutra) */
    if (strstr(missao, "Verde") != NULL) {
        return contarTerritoriosPorCor(mapa, tamanho, "Verde") == 0;
    }

    /* Missao de acumulo de tropas em um unico territorio */
    if (strstr(missao, "tropas em um unico territorio") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (mapa[i].tropas > 10) {
                return 1;
            }
        }
        return 0;
    }

    /* Missao de conquista de TODOS os territorios */
    if (strstr(missao, "todos os 5") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (contarTerritoriosPorCor(mapa, tamanho, mapa[i].cor) == 5) {
                return 1;
            }
        }
        return 0;
    }

    /* Missao de dominar pelo menos 4 territorios sob uma unica cor */
    if (strstr(missao, "4 territorios") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (contarTerritoriosPorCor(mapa, tamanho, mapa[i].cor) >= 4) {
                return 1;
            }
        }
        return 0;
    }

    /* Missao de dominar pelo menos 3 territorios sob uma unica cor */
    if (strstr(missao, "3 territorios") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (contarTerritoriosPorCor(mapa, tamanho, mapa[i].cor) >= 3) {
                return 1;
            }
        }
        return 0;
    }

    return 0; // missao nao reconhecida
}

/* ============================================================================
 *  atacar
 *  Simula um ataque entre dois territorios. Recebe os territorios POR
 *  REFERENCIA (ponteiros), pois seus campos sao alterados.
 *  - Valida que o atacante so ataca territorios inimigos.
 *  - Usa rand() para simular a rolagem de dois dados (valores de 1 a 6).
 *  - Se o atacante vencer: transfere a cor e metade das tropas ao defensor.
 *  - Caso contrario: o atacante perde uma tropa.
 * ==========================================================================*/
void atacar(Territorio* atacante, Territorio* defensor) {

    /* Validacao: nao e permitido atacar territorio da mesma cor */
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Ataque invalido: nao e possivel atacar um territorio aliado.\n");
        return;
    }

    /* Validacao: o atacante precisa de pelo menos 2 tropas para atacar */
    if (atacante->tropas < 2) {
        printf("Ataque invalido: o atacante precisa de pelo menos 2 tropas.\n");
        return;
    }

    /* Rolagem de dados (1 a 6) */
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;
    printf("Dado do atacante: %d | Dado do defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        /* Atacante venceu: conquista o territorio defensor */
        int metade = atacante->tropas / 2;     // metade das tropas do atacante
        printf(">> %s venceu o ataque e conquistou %s!\n",
               atacante->nome, defensor->nome);
        strcpy(defensor->cor, atacante->cor);  // transfere a cor
        defensor->tropas = metade;             // transfere metade das tropas
        atacante->tropas -= metade;            // atacante perde as tropas enviadas
    } else {
        /* Atacante perdeu: perde uma tropa */
        printf(">> %s defendeu o ataque. O atacante perdeu 1 tropa.\n", defensor->nome);
        atacante->tropas -= 1;
    }
}

/* ============================================================================
 *  exibirMapa
 *  Exibe o estado atual de todos os territorios do mapa.
 * ==========================================================================*/
void exibirMapa(Territorio* mapa, int tamanho) {
    printf("---------------------------------------------\n");
    printf("%-12s %-10s %s\n", "TERRITORIO", "COR", "TROPAS");
    printf("---------------------------------------------\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%-12s %-10s %d\n", mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("---------------------------------------------\n");
}

/* ============================================================================
 *  liberarMemoria
 *  Libera toda a memoria alocada dinamicamente (mapa e missoes dos jogadores),
 *  evitando vazamentos de memoria.
 * ==========================================================================*/
void liberarMemoria(Territorio* mapa, char* missaoJogador1, char* missaoJogador2) {
    free(mapa);
    free(missaoJogador1);
    free(missaoJogador2);
    printf("\nMemoria liberada com sucesso.\n");
}

/* ============================================================================
 *  main
 *  Coordena a execucao: cria o mapa, sorteia e exibe as missoes, executa os
 *  turnos de ataque, verifica silenciosamente as missoes e declara o vencedor.
 * ==========================================================================*/
int main(void) {
    srand(time(NULL));  // inicializa a semente do gerador de numeros aleatorios

    /* ----- Vetor de missoes: pelo menos 5 descricoes diferentes ----- */
    char* missoes[TOTAL_MISSOES] = {
        "Conquistar pelo menos 3 territorios sob uma unica cor",
        "Conquistar pelo menos 4 territorios sob uma unica cor",
        "Eliminar completamente o exercito Verde do mapa",
        "Acumular mais de 10 tropas em um unico territorio",
        "Conquistar todos os 5 territorios sob uma unica cor"
    };

    /* ----- Alocacao dinamica do mapa de territorios (calloc) ----- */
    Territorio* mapa = (Territorio*) calloc(TAM_MAPA, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria para o mapa.\n");
        return 1;
    }

    /* Inicializacao dos territorios do mapa */
    strcpy(mapa[0].nome, "America"); strcpy(mapa[0].cor, "Azul");     mapa[0].tropas = 5;
    strcpy(mapa[1].nome, "Europa");  strcpy(mapa[1].cor, "Vermelho"); mapa[1].tropas = 4;
    strcpy(mapa[2].nome, "Asia");    strcpy(mapa[2].cor, "Verde");    mapa[2].tropas = 3;
    strcpy(mapa[3].nome, "Africa");  strcpy(mapa[3].cor, "Vermelho"); mapa[3].tropas = 3;
    strcpy(mapa[4].nome, "Oceania"); strcpy(mapa[4].cor, "Azul");     mapa[4].tropas = 4;

    /* ----- Alocacao dinamica das missoes de cada jogador (malloc) ----- */
    char* missaoJogador1 = (char*) malloc(TAM_MISSAO * sizeof(char));
    char* missaoJogador2 = (char*) malloc(TAM_MISSAO * sizeof(char));
    if (missaoJogador1 == NULL || missaoJogador2 == NULL) {
        printf("Erro ao alocar memoria para as missoes.\n");
        free(mapa);
        return 1;
    }

    /* Cores que representam cada jogador */
    char corJogador1[10] = "Azul";
    char corJogador2[10] = "Vermelho";

    /* ----- Sorteio das missoes (passagem por referencia) ----- */
    atribuirMissao(missaoJogador1, missoes, TOTAL_MISSOES);
    atribuirMissao(missaoJogador2, missoes, TOTAL_MISSOES);

    printf("=============================================\n");
    printf("        WAR ESTRUTURADO - NIVEL MESTRE       \n");
    printf("=============================================\n\n");

    /* Exibicao da missao apenas uma vez, no inicio (passagem por valor) */
    printf("Jogador 1 (%s):\n", corJogador1);
    exibirMissao(missaoJogador1);
    printf("Jogador 2 (%s):\n", corJogador2);
    exibirMissao(missaoJogador2);

    printf("\n--- Mapa inicial ---\n");
    exibirMapa(mapa, TAM_MAPA);

    /* ----- Loop principal de turnos ----- */
    int turno = 1;
    int vencedor = 0;  // 0 = nenhum, 1 = jogador 1, 2 = jogador 2

    while (turno <= MAX_TURNOS && vencedor == 0) {
        printf("\n=============== TURNO %d ===============\n", turno);

        /* O jogador ativo alterna a cada turno */
        char* corAtiva = (turno % 2 == 1) ? corJogador1 : corJogador2;
        printf("Vez do jogador %s\n", corAtiva);

        /* Busca um territorio do jogador ativo com tropas suficientes (atacante) */
        int idxAtacante = -1;
        for (int i = 0; i < TAM_MAPA; i++) {
            if (strcmp(mapa[i].cor, corAtiva) == 0 && mapa[i].tropas >= 2) {
                idxAtacante = i;
                break;
            }
        }

        /* Busca um territorio inimigo para ser atacado (defensor) */
        int idxDefensor = -1;
        if (idxAtacante != -1) {
            for (int i = 0; i < TAM_MAPA; i++) {
                if (strcmp(mapa[i].cor, corAtiva) != 0) {
                    idxDefensor = i;
                    break;
                }
            }
        }

        /* Executa o ataque, se houver alvo valido */
        if (idxAtacante != -1 && idxDefensor != -1) {
            printf("%s (%s) ataca %s (%s)\n",
                   mapa[idxAtacante].nome, mapa[idxAtacante].cor,
                   mapa[idxDefensor].nome, mapa[idxDefensor].cor);
            atacar(&mapa[idxAtacante], &mapa[idxDefensor]);
        } else {
            printf("Nenhum ataque possivel neste turno.\n");
        }

        /* Estado do mapa apos o turno */
        exibirMapa(mapa, TAM_MAPA);

        /* ----- Verificacao SILENCIOSA das missoes ao final do turno ----- */
        if (verificarMissao(missaoJogador1, mapa, TAM_MAPA)) {
            vencedor = 1;
        } else if (verificarMissao(missaoJogador2, mapa, TAM_MAPA)) {
            vencedor = 2;
        }

        turno++;
    }

    /* ----- Resultado final ----- */
    printf("\n=============================================\n");
    if (vencedor == 1) {
        printf("VITORIA do Jogador 1 (%s)!\n", corJogador1);
        printf("Missao cumprida: %s\n", missaoJogador1);
    } else if (vencedor == 2) {
        printf("VITORIA do Jogador 2 (%s)!\n", corJogador2);
        printf("Missao cumprida: %s\n", missaoJogador2);
    } else {
        printf("Fim da simulacao: nenhum jogador cumpriu a missao a tempo.\n");
    }
    printf("=============================================\n");

    /* ----- Liberacao de toda a memoria alocada dinamicamente ----- */
    liberarMemoria(mapa, missaoJogador1, missaoJogador2);

    return 0;
}
