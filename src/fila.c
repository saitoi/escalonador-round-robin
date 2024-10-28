#include <stdio.h>
#include <stdlib.h>

#include "../include/fila.h"

Fila *aloca_fila(void) {
    Fila *fila = NULL;
    controla_erro_alocacao((fila = (Fila *) malloc(sizeof(Fila))));
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

    if (s == NULL) {
        tratar_erro_alocacao("Falha ao alocar struct.\n");
        return NULL;
    }

    controla_erro_alocacao((no = (No *) malloc(sizeof(No))));
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
        printf("Fila vazia.\n");
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

void imprime_fila(Fila *fila) {
    No *atual = fila->inicio;

    if (fila_vazia(fila)) {
        printf("\n");
        return;
    }

    while (atual != NULL) {
        printf("P%d ", atual->processo.pid);
        atual = atual->prox;
    }
    printf("\n");
}

void imprime_fila_formatada(const char *nome_fila, Fila *fila) {
    printf("%s: ", nome_fila);
    imprime_fila(fila);
}

void imprime_filas(Fila *fila_alta_prioridade, Fila *fila_baixa_prioridade, Fila *fila_disco, Fila *fila_fita, Fila *fila_impressora) {
    printf("\n");
    imprime_fila_formatada("Fila de alta prioridade", fila_alta_prioridade);
    imprime_fila_formatada("Fila de baixa prioridade", fila_baixa_prioridade);
    imprime_fila_formatada("Fila de disco", fila_disco);
    imprime_fila_formatada("Fila de fita", fila_fita);
    imprime_fila_formatada("Fila de impressora", fila_impressora);
}
