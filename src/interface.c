#include <stdio.h>
#include <stdlib.h>

#include "../include/fila.h"
#include "../include/auxiliar.h"
#include "../include/escalonador.h"
#include "../include/processo.h"
#include "../include/interface.h"

void imprime_menu(void) {
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║      SIMULADOR ROUND ROBIN COM FEEDBACK      ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    printf("║  1. Carregar dados de arquivo externo (CSV). ║\n");
    printf("║  2. Gerar dados  \"aleatoriamente\".           ║\n");
    printf("║  3. Sair.                                    ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
}

// TODO: Reduzir parâmetros dessa função
void processa_menu(Fila *fila_alta_prioridade, Fila *fila_baixa_prioridade, Fila *fila_disco, Fila *fila_fita) {
    Processo *processos = NULL;
    int opcao = -1;

    imprime_menu();

    valida_entrada_inteiro("Escolha uma opção", &opcao, 1, 3);

    switch (opcao) {
        case 1:
            processos = criar_processos_csv("processos.csv");
            if (!processos) {
                tratar_erro_alocacao("Erro ao carregar processos do arquivo.\n");
                exit(1);
            }
            break;
        case 2:
            processos = criar_processos_aleatorios();
            if (!processos) {
                tratar_erro_alocacao("Erro ao alocar processos iniciais.\n");
                exit(1);
            }
            break;
        case 3:
            printf("Encerrando o simulador. Até logo!\n");
            return;
        default:
            printf("Opção inválida. Tente novamente.\n\n");
            return;
    }

    escalonador(processos,
                fila_alta_prioridade,
                fila_baixa_prioridade,
                fila_disco,
                fila_fita);
}



void imprime_todas_filas(Fila *fila_alta_prioridade, Fila *fila_baixa_prioridade, Fila *fila_disco, Fila *fila_fita) {
    printf("\n");
    imprime_fila("  Alta Prioridade  ", fila_alta_prioridade);
    imprime_fila("  Baixa Prioridade ", fila_baixa_prioridade);
    imprime_fila("       Disco       ", fila_disco);
    imprime_fila("️       Fita        ", fila_fita);
}

void imprime_fila(const char *nome_fila, Fila *fila) {
    if (fila == NULL || fila_vazia(fila)) {
        printf("╔═════════════════════════════════════╗\n");
        printf("║  %s: Vazia                          ║\n", nome_fila);
        printf("╚═════════════════════════════════════╝\n");
        return;
    }

    int count = 0;
    No *current = fila->inicio;
    while (current != NULL) {
        count++;
        current = current->prox;
    }

    printf("╔═════════════════════════════════════╗\n");
    printf("║  %s:                                ║\n", nome_fila);
    printf("╠═════════════════════════════════════╣\n");
    
    // Imprime os elementos da fila em estilo gráfico
    printf("║  ┌─────────");
    for (int i = 1; i < count; i++) {
        printf("┬─────────");
    }
    printf("┐     ║\n");

    current = fila->inicio;
    if (current != NULL) {
        printf("║  │   P%d    ", current->processo.pid);
        current = current->prox;
        while (current != NULL) {
            printf("│   P%d    ", current->processo.pid);
            current = current->prox;
        }
        printf("│     ║\n");
    }

    printf("║  └─────────");
    for (int i = 1; i < count; i++) {
        printf("┴─────────");
    }
    printf("┘     ║\n");

    printf("╚═════════════════════════════════════╝\n");
}

