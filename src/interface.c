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

void imprime_instante(int tempo_atual) {
    printf("\n┌────────────────────────────────────┐\n");
    printf("│         >>> INSTANTE %02d <<<        │\n", tempo_atual);
    printf("└────────────────────────────────────┘\n");
}

// TODO: Reduzir parâmetros dessa função
void processa_menu(Fila *fila_alta_prioridade, Fila *fila_baixa_prioridade, Fila *fila_disco, Fila *fila_fita) {
    ListaProcessos *lista = malloc(sizeof(ListaProcessos));
    int opcao = -1;
    if (lista == NULL) {
        tratar_erro_alocacao("Erro na alocação de memória para lista de processos.\n");
        exit(-1);
    }


    imprime_menu();

    valida_entrada_inteiro("Escolha uma opção", &opcao, 1, 3);

    switch (opcao) {
        case 1:
            *lista = criar_lista_processos_csv("processos.csv");
            if (!lista->processos) {
                tratar_erro_alocacao("Erro ao carregar processos do arquivo.\n");
                exit(1);
            }
            break;
        case 2:
            *lista = criar_lista_processos_aleatorios();
            if (!lista->processos) {
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

    escalonador(lista,
                fila_alta_prioridade,
                fila_baixa_prioridade,
                fila_disco,
                fila_fita);
}

void imprime_todas_filas(Fila *fila_alta_prioridade, Fila *fila_baixa_prioridade, Fila *fila_disco, Fila *fila_fita) {
    printf("\n");
    imprime_fila(" ALTA PRIORIDADE  ", fila_alta_prioridade);
    imprime_fila(" BAIXA PRIORIDADE ", fila_baixa_prioridade);
    imprime_fila(" DISCO ", fila_disco);
    imprime_fila("️ FITA ", fila_fita);
}

void imprime_fila(const char *nome_fila, Fila *fila) {
    if (fila == NULL || fila_vazia(fila)) {
        printf("%s: Vazia\n\n", nome_fila);
        return;
    }

    printf("%s:\n", nome_fila);

    No *current = fila->inicio;

    // Imprime o topo da fila
    printf("  ┌─────────");
    while (current->prox != NULL) {
        printf("┬─────────");
        current = current->prox;
    }
    printf("┐\n");

    // Reseta o ponteiro para o início da fila
    current = fila->inicio;

    // Imprime os processos na fila
    printf("  │   P%d    ", current->processo.pid);
    current = current->prox;
    while (current != NULL) {
        printf("│   P%d    ", current->processo.pid);
        current = current->prox;
    }
    printf("│\n");

    // Imprime a base da fila
    printf("  └─────────");
    current = fila->inicio;
    while (current->prox != NULL) {
        printf("┴─────────");
        current = current->prox;
    }
    printf("┘\n");
}
