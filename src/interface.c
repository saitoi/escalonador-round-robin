#include <stdio.h>
#include <stdlib.h>

#include "../include/fila.h"
#include "../include/utilitarios.h"
#include "../include/escalonador.h"
#include "../include/processo.h"
#include "../include/interface.h"

/* ************* PROCESSAMENTO DO MENU ************* */

void processa_menu(ListaProcessos *lista_processos) {
    char opcao;

    imprime_menu();
    obter_entrada_caractere("Escolha uma opcao", &opcao, '1', '3');

    switch (opcao) {
        /* 1: Processos sao extraidos de um CSV */
        case '1':
            *lista_processos = criar_lista_processos_csv("processos.csv");
            break;
        /* 2: Processos sao gerados aleatoriamente */
        case '2':
            *lista_processos = criar_lista_processos_aleatorios();
            break;
        case '3':
        /* 3: Usuario escolheu encerrar o programa */
            printf("Encerrando o simulador. Ate logo!\n");
            return;
        default:
        /* Opcao invalida, encerrando o programa.. */
            printf("Opcao invalida. Tente novamente.\n\n");
            return;
    }
}

/* ************* IMPRESSAO DE INTERFACES BASICAS E INDEPENDENTES ************* */

void imprime_menu(void) {
    printf("+----------------------------------------------+\n");
    printf("|      SIMULADOR ROUND ROBIN COM FEEDBACK      |\n");
    printf("+----------------------------------------------+\n");
    printf("|  1. Carregar dados de arquivo externo (CSV). |\n");
    printf("|  2. Gerar dados aleatoriamente.              |\n");
    printf("|  3. Sair.                                    |\n");
    printf("+----------------------------------------------+\n");
}

void imprime_instante(int tempo_atual) {
    printf("\n+------------------------------------+\n");
    printf("|         >>> INSTANTE %02d <<<        |\n", tempo_atual);
    printf("+------------------------------------+\n");
}

void imprime_fim_escalonador(void) {
    printf("\n[+] Todos os processos foram finalizados com sucesso.\n");
    printf("\n+-----------------------------------------+\n");
    printf("|           FIM DO ESCALONAMENTO          |\n");
    printf("+-----------------------------------------+\n");
    printf("\nTempos de turnaround dos processos:");
}

void imprime_todas_filas(ListaFila *lista_filas) {
    printf("\n");
    imprime_fila("ALTA PRIORIDADE  ", &lista_filas->filas[FILA_ALTA_PRIORIDADE]);
    imprime_fila("BAIXA PRIORIDADE ", &lista_filas->filas[FILA_BAIXA_PRIORIDADE]);
    imprime_fila("DISCO ", &lista_filas->filas[FILA_DISCO]);
    imprime_fila("FITA ", &lista_filas->filas[FILA_FITA]);
}

void imprime_tabela_processos(ListaProcessos *lista_processos) {
    if (lista_processos->quantidade > 0) {
        printf("\n================================ PROCESSOS ======================================\n");
        printf(" PID\tTempo de Inicio\t\tTempo de Servico\tE/S (Tempo de Inicio)\n");
        printf("---------------------------------------------------------------------------------\n");

        for (int i = 0; i < lista_processos->quantidade; i++) {
            printf(" P%d\t\t%d\t\t\t%d\t\t", 
                lista_processos->processos[i].pid, 
                lista_processos->processos[i].instante_chegada,
                lista_processos->processos[i].tempo_cpu);

            if (lista_processos->processos[i].num_operacoes_io == 0) {
                printf("Nenhuma operacao de E/S.");
            } else {
                for (int j = 0; j < QUANTIDADE_TIPOS_IO; j++) {
                    int tempo_inicio_io = lista_processos->processos[i].operacoes_io[j].tempo_inicio;
                    if (tempo_inicio_io == -1)
                        continue;
                    printf("%s (%d)", seleciona_tipo_io(lista_processos->processos[i].operacoes_io[j].tipo_io), tempo_inicio_io);

                    if (j < lista_processos->processos[i].num_operacoes_io - 1) {
                        printf(", ");
                    }
                }
            }
            printf("\n");
        }

        printf("=================================================================================\n");
    } else {
        printf("Sem processos.\n");
    }
}

void imprime_turnaround_processos(ListaProcessos lista_processos) {
    float turnaround_medio = 0,
          tempo_espera_medio = 0;

    printf("\n======================== TURNAROUND ============================\n");
    printf(" PID\tTempo de Turnaround\tTempo de Espera\n");
    printf("----------------------------------------------------------------\n");

    for (int i = 0; i < lista_processos.quantidade; i++) {
        printf(" P%d\t\t%d u.t.\t\t %d u.t.\n", 
            lista_processos.processos[i].pid, 
            lista_processos.processos[i].tempo_turnaround, 
            lista_processos.processos[i].tempo_turnaround - lista_processos.processos[i].tempo_cpu);
        turnaround_medio += (float) lista_processos.processos[i].tempo_turnaround;
        tempo_espera_medio += (float) lista_processos.processos[i].tempo_turnaround - lista_processos.processos[i].tempo_cpu;
    }

    printf("================================================================\n");
    printf(" Turnaround medio: %.2f u.t.\n", turnaround_medio / lista_processos.quantidade);
    printf(" Tempo de espera medio: %.2f u.t.\n", tempo_espera_medio / lista_processos.quantidade);
}

void imprime_fila(const char *nome_fila, Fila *fila) {
    if (fila == NULL || fila_vazia(fila)) {
        printf("%s: Vazia\n\n", nome_fila);
        return;
    }

    printf("%s:\n", nome_fila);

    No *current = fila->inicio;

    // Imprime o topo da fila
    printf("  +---------");
    while (current->prox != NULL) {
        printf("+---------");
        current = current->prox;
    }
    printf("+\n");

    // Reseta o ponteiro para o inicio da fila
    current = fila->inicio;

    // Imprime os processos na fila
    printf("  |   P%d    ", current->processo->pid);
    current = current->prox;
    while (current != NULL) {
        printf("|   P%d    ", current->processo->pid);
        current = current->prox;
    }
    printf("|\n");

    // Imprime a base da fila
    printf("  +---------");
    current = fila->inicio;
    while (current->prox != NULL) {
        printf("+---------");
        current = current->prox;
    }
    printf("+\n");
}
