#include <stdio.h>

#include "../include/auxiliar.h"

void tratar_erro_alocacao(const char *mensagem) {
    fprintf(stderr, "%s\n", mensagem);
    exit(1);
}

int valida_entrada_inteiro(const char *mensagem, int *variavel, int min, int max) {
    int valor;
    char ch;

    printf("%s (atual: %d): ", mensagem, *variavel);
    if (scanf("%d", &valor) != 1) {
        printf("Caractere inválido.", mensagem_erro);
        while ((ch = getchar()) != '\n' && ch != EOF);
        return 0;
    }

    if (valor < min || valor > max) {
        printf("Entrada inválida, escolha um valor dentro do intervalo.\n", mensagem_erro);
        while ((ch = getchar()) != '\n' && ch != EOF);
        return 0;
    }

    *variavel = valor;
    return 1;
}

void quicksort(OperacaoES *operacoes_es, int inicio, int fim) {
    int pos_pivo = 0;

    if (fim > inicio) {
        pos_pivo = particiona(operacoes_es, inicio, fim);
        quicksort(operacoes_es, inicio, pos_pivo - 1);
        quicksort(operacoes_es, pos_pivo + 1, fim);
    }
}

int particiona(OperacaoES *operacoes_es, int inicio, int fim) {
    int pos_pivo = 0;
    OperacaoES pivo;
    int i = inicio, j = fim;

    pos_pivo = rand() % (fim + 1 - inicio) + inicio;
    pivo = operacoes_es[pos_pivo];

    operacoes_es[pos_pivo] = operacoes_es[inicio];
    operacoes_es[inicio] = pivo;

    while (i < j) {
        while (i < fim && operacoes_es[i].tempo_inicio <= pivo.tempo_inicio) i++;
        while (j > inicio && operacoes_es[j].tempo_inicio > pivo.tempo_inicio) j--;
        if (i < j) troca(&operacoes_es[i], &operacoes_es[j]);
    }

    operacoes_es[inicio] = operacoes_es[j];
    operacoes_es[j] = pivo;
    return j;
}

void troca(OperacaoES *a, OperacaoES *b) {
    OperacaoES temp = *b;
    *b = *a;
    *a = temp;
}
