#include <stdio.h>
#include <string.h>

// ============================================================
//  Nivel Novato: Cadastro Inicial dos Territorios
// ============================================================

// Definicao da struct que representa um territorio do mapa.
typedef struct {
    char nome[30];   // Nome do territorio
    char cor[10];    // Cor do exercito dominante
    int tropas;      // Numero de tropas
} Territorio;

int main(void) {
    // Vetor estatico capaz de armazenar 5 territorios.
    Territorio mapa[5];
    int i;

    printf("=========================================\n");
    printf("   CADASTRO INICIAL DOS TERRITORIOS\n");
    printf("=========================================\n\n");

    // ---------- Leitura dos dados ----------
    for (i = 0; i < 5; i++) {
        printf("--- Cadastro do Territorio %d ---\n", i + 1);

        // Leitura do nome (pode conter espacos) com fgets.
        printf("Nome do territorio: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        // Remove o '\n' deixado pelo fgets, se houver.
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        // Leitura da cor do exercito (uma palavra) com fgets.
        printf("Cor do exercito: ");
        fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        // Leitura do numero de tropas com scanf.
        printf("Numero de tropas: ");
        scanf("%d", &mapa[i].tropas);

        // Limpa o buffer de entrada apos o scanf para o proximo fgets
        // funcionar corretamente.
        getchar();

        printf("\n");
    }

    // ---------- Exibicao do estado atual do mapa ----------
    printf("\n=========================================\n");
    printf("        ESTADO ATUAL DO MAPA\n");
    printf("=========================================\n\n");

    for (i = 0; i < 5; i++) {
        printf("Territorio %d\n", i + 1);
        printf("  Nome ....: %s\n", mapa[i].nome);
        printf("  Cor .....: %s\n", mapa[i].cor);
        printf("  Tropas ..: %d\n", mapa[i].tropas);
        printf("-----------------------------------------\n");
    }

    return 0;
}