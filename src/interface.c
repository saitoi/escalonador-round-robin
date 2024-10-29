#include <stdio.h>
#include <stdlib.h>

#include "../include/fila.h"
#include "../include/auxiliar.h"
#include "../include/escalonador.h"
#include "../include/processo.h"
#include "../include/interface.h"

void imprime_menu(void) {
    printf("╔════════════════════════════════════════════╗\n");
    printf("║     SIMULADOR ROUND ROBIN COM FEEDBACK     ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    printf("║ 1. Criar um processo.                      ║\n");
    printf("║ 2. Executar o escalonador.                 ║\n");
    printf("║ 3. Sair                                    ║\n");
    printf("╚════════════════════════════════════════════╝\n");
}

// TODO: Reduzir parâmetros dessa função
void processa_menu(Processo *processos, Fila *fila_alta_prioridade, Fila *fila_baixa_prioridade, Fila *fila_disco, Fila *fila_fita, Fila *fila_impressora)
{
    int opcao;
    int contador_processos = 0;

    imprime_menu();

    valida_entrada_inteiro(
        "Escolha uma opção",
        &opcao,
        1,
        3);

    switch (opcao) {
    case 1: {
        if (contador_processos < MAXIMO_PROCESSOS) {
            Processo *novo_processo_usuario = configurar_processo_usuario();
            processos[contador_processos] = *novo_processo_usuario;
            contador_processos++;
            free(novo_processo_usuario); // Libera a memória alocada
        }
        else {
            printf("Número máximo de processos alcançado.\n");
        }
        break;
    }
    case 2:
        escalonador(processos,
                    fila_alta_prioridade,
                    fila_baixa_prioridade,
                    fila_disco,
                    fila_fita,
                    fila_impressora);
        break;
    case 3:
        printf("Encerrando o simulador. Até logo!\n");
        break;
    default:
        printf("Opção inválida. Tente em outra vez.\n\n");
    }
}

void imprime_todas_filas(Fila *fila_alta_prioridade, Fila *fila_baixa_prioridade, Fila *fila_disco, Fila *fila_fita, Fila *fila_impressora) {
    printf("\n");
    imprime_fila("  Alta Prioridade", fila_alta_prioridade);
    imprime_fila("  Baixa Prioridade", fila_baixa_prioridade);
    imprime_fila("  Disco", fila_disco);
    imprime_fila("️  Fita", fila_fita);
    imprime_fila("️  Impressora", fila_impressora);
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

    // Cabeçalho da fila
    printf("╔═════════════════════════════════════╗\n");
    printf("║  %s:                                ║\n", nome_fila);
    printf("╠═════════════════════════════════════╣\n");
    
    // Parte superior da caixa
    printf("║  ┌─────────");
    for (int i = 1; i < count; i++) {
        printf("┬─────────");
    }
    printf("┐  ║\n");

    // Impressão dos processos
    current = fila->inicio;
    printf("║  │   P%d    ", current->processo.pid);
    current = current->prox;
    while (current != NULL) {
        printf("│   P%d    ", current->processo.pid);
        current = current->prox;
    }
    printf("│  ║\n");

    // Parte inferior da caixa
    printf("║  └─────────");
    for (int i = 1; i < count; i++) {
        printf("┴─────────");
    }
    printf("┘  ║\n");

    printf("╚═════════════════════════════════════╝\n");
}
