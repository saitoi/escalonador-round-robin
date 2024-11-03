#include <stdio.h>
#include <stdlib.h>

#include "../include/fila.h"
#include "../include/auxiliar.h"

Fila *aloca_fila(void) {
    Fila *fila = NULL;

    fila = (Fila *) malloc(sizeof(Fila));

    if (fila == NULL) {
        tratar_erro_alocacao("Erro na alocação da fila.\n");
        return NULL;
    }

    return fila;
}

void inicializa_fila(Fila *fila) {
    fila->inicio = fila->fim = NULL;
}

int fila_vazia(Fila *fila) {
    if (fila->inicio == NULL) {
        fila->fim = NULL;
        return 1;
    }
    return 0;
}

No *aloca_no(void) {
    No *no = (No *) malloc(sizeof(No));

    if (!no) {
        tratar_erro_alocacao("Falha ao alocar struct.\n");
        return NULL;
    }

    return no;
}

void enfileira(Fila *fila, Processo processo) {
    No *novo = aloca_no();

    novo->processo = processo;
    novo->prox = NULL;

    if (fila_vazia(fila))
        fila->inicio = fila->fim = novo;
    else {
        fila->fim->prox = novo;
        fila->fim = novo;
    }
}

void enfileira_inicio(Fila *fila, Processo processo) {
    No *novo = aloca_no();

    novo->processo = processo;
    novo->prox = NULL;

    if (fila_vazia(fila))
        fila->inicio = fila->fim = novo;
    else {
        novo->prox = fila->inicio;
        fila->inicio = novo;
    }
}

Processo desenfileira(Fila *fila) {
    No *aux = NULL;
    Processo processo;

    if (fila_vazia(fila)) {
        printf("Fila vazia.\n"); /* Conflito de lógica (Erro no código ou entradas): exit(-1) */
        exit(1);
    }

    aux = fila->inicio;
    processo = aux->processo;
    fila->inicio = aux->prox;

    if (fila_vazia(fila))
        fila->fim = NULL;

    free(aux);

    return processo;
}

void esvazia_fila(Fila *fila) {
    No *aux = NULL;

    while (fila->inicio != NULL) {
        aux = fila->inicio;
        fila->inicio = fila->inicio->prox;
        free(aux);
    }
}
