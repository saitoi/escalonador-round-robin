#include <stdio.h>
#include <stdlib.h>

#include "../include/auxiliar.h"
#include "../include/processo.h"

void tratar_erro_alocacao(const char *mensagem) {
    fprintf(stderr, "%s\n", mensagem);
    exit(1);
}

void valida_entrada_inteiro(const char *mensagem, int *variavel, int min, int max) {
    int opcao_valida = 1;
    int valor;
    char ch;

    do {
        printf("%s (atual: %d): ", mensagem, *variavel);
        if (scanf("%d", &valor) != 1) {
            printf("Caractere inválido.");
            while ((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }

        if (valor < min || valor > max) {
            printf("Entrada inválida, escolha um valor dentro do intervalo.\n");
            while ((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }

        *variavel = valor;
        opcao_valida = 1;

    } while(!opcao_valida);
}

void quicksort(OperacaoIO *operacoes_io, int inicio, int fim) {
    int pos_pivo = 0;

    if (fim > inicio) {
        pos_pivo = particiona(operacoes_io, inicio, fim);
        quicksort(operacoes_io, inicio, pos_pivo - 1);
        quicksort(operacoes_io, pos_pivo + 1, fim);
    }
}

int particiona(OperacaoIO *operacoes_io, int inicio, int fim) {
    int pos_pivo = 0;
    OperacaoIO pivo;
    int i = inicio, j = fim;

    pos_pivo = rand() % (fim + 1 - inicio) + inicio;
    pivo = operacoes_io[pos_pivo];

    operacoes_io[pos_pivo] = operacoes_io[inicio];
    operacoes_io[inicio] = pivo;

    while (i < j) {
        while (i < fim && operacoes_io[i].tempo_inicio <= pivo.tempo_inicio) i++;
        while (j > inicio && operacoes_io[j].tempo_inicio > pivo.tempo_inicio) j--;
        if (i < j) troca(&operacoes_io[i], &operacoes_io[j]);
    }

    operacoes_io[inicio] = operacoes_io[j];
    operacoes_io[j] = pivo;
    return j;
}

void troca(OperacaoIO *a, OperacaoIO *b) {
    OperacaoIO temp = *b;
    *b = *a;
    *a = temp;
}
