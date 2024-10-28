#include <stdio.h>
#include <stdlib.h>

#include "auxiliar.c"
#include "../include/interface.h"
#include "../include/escalonador.h"

void imprime_menu(void) {
    printf("==============================================\n");
    printf("      SIMULADOR ROUND ROBIN COM FEEDBACK       \n");
    printf("==============================================\n");
    printf("1. Criar um processo.\n");
    printf("2. Executar o escalonador.\n");
    printf("3. Sair\n");
    printf("==============================================\n");
}

// TODO: Reduzir parâmetros dessa função
void processa_menu(Processo *processos, Fila *fila_alta_prioridade, Fila *fila_baixa_prioridade, Fila *fila_disco, Fila *fila_fita, Fila *fila_impressora) {
    int *opcao;

    imprime_menu();

    valida_entrada_inteiro(
        "Escolha uma opção",
        &opcao,
        1,
        3
    )

    switch(*opcao) {
        case 1:
            configurar_processo_usuario();
            break;
        case 2:
            processa_menu(processos,
                        &fila_alta_prioridade,
                        &fila_baixa_prioridade,
                        &fila_disco, 
                        &fila_fita, 
                        &fila_impressora);
            break;
        case 3:
            printf("Encerrando o simulador. Até logo!\n");
            return;
        default:
            printf("Opção inválida. Tente novamente.\n\n");
    }
}
