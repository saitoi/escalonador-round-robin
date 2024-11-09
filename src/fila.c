#include <stdio.h>
#include <stdlib.h>

#include "../include/fila.h"
#include "../include/auxiliar.h"

/* ************* INICIALIZAÇÃO ************* */

void inicializa_fila(Fila *fila) {
    fila->inicio = fila->fim = NULL;
}

void inicializa_lista_filas(ListaFila *lista) {
    for (int i = 0; i < NUM_FILAS; ++i) {
        inicializa_fila(&lista->filas[i]);
    }
}

Fila *aloca_fila(void) {
    Fila *fila = NULL;

    fila = (Fila *) malloc(sizeof(Fila));

    if (!fila) {
        enviar_mensagem_erro("Erro na alocação da fila.\n");
        return NULL;
    }

    return fila;
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
        enviar_mensagem_erro("Falha ao alocar struct.\n");
        return NULL;
    }

    return no;
}

void enfileira(Fila *fila, Processo *processo) {
    No *novo = aloca_no();
    if (!novo) {
        enviar_mensagem_erro("Erro ao alocar memória para novo nó.\n");
        return;
    }

    /* Armazenando o ponteiro para o processo */
    novo->processo = processo;
    novo->prox = NULL;

    if (fila_vazia(fila))
        fila->inicio = fila->fim = novo;
    else {
        fila->fim->prox = novo;
        fila->fim = novo;
    }
}


void enfileira_inicio(Fila *fila, Processo *processo) {
    No *novo = aloca_no();
    if (!novo) {
        enviar_mensagem_erro("Erro ao alocar memória para novo nó.\n");
        return;
    }

    /* Armazenando o ponteiro para o processo */
    novo->processo = processo;
    novo->prox = fila->inicio;

    fila->inicio = novo;

    if (fila->fim == NULL) {
        fila->fim = novo;
    }
}


Processo *desenfileira(Fila *fila) {
    No *aux = NULL;
    Processo *processo;

    if (fila_vazia(fila)) {
        printf("Fila vazia.\n"); /* Conflito de lógica (Erro no código ou entradas): exit(-1) */
        exit(1);
    }

    aux = fila->inicio;
    processo = aux->processo; // Obtendo o ponteiro para o processo
    fila->inicio = aux->prox;

    if (fila->inicio == NULL) // Se a fila ficou vazia após desenfileirar
        fila->fim = NULL;

    free(aux); // Liberando o nó, mas não o processo

    return processo; // Retornando o ponteiro para o processo original
}

void esvazia_fila(Fila *fila) {
    No *aux = NULL;

    while (fila->inicio != NULL) {
        aux = fila->inicio;
        fila->inicio = fila->inicio->prox;
        free(aux);
    }
    fila->fim = NULL;
}
