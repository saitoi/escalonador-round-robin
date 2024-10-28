#ifndef FILA_H
#define FILA_H

#include <stdio.h>
#include <stdlib.h>

#include "processo.h"

/* Estrutura para armazenar um no */
typedef struct _No {
    Processo processo; /* Processo armazenado no no */
    struct _No *prox; /* Ponteiro para o proximo no */
} No;

/* Estrutura para armazenar uma fila */
typedef struct _Fila {
    No *inicio; /* Ponteiro para o inicio da fila */
    No *fim; /* Ponteiro para o fim da fila */
} Fila;

/* Funcao para alocar o espaço de uma fila */
/* Nao recebe parametros */
/* Retorna um ponteiro para a fila alocada */
Fila *aloca_fila(void);

/* Funcao para inicializar uma fila */
/* Recebe a fila a ser inicializada */
/* Nao retorna valores */
void inicializa_fila(Fila *fila);

/* Funcao para verificar se a fila esta vazia */
/* Recebe a fila a ser verificada */
/* Retorna 1 caso a fila esteja vazia, 0 caso contrario */
int fila_vazia(Fila *fila);

/* Funcao para alocar o espaco de um no */
/* Nao recebe parametros */
/* Retorna um ponteiro para o no alocado */
No *aloca_no(void);

/* Funcao para enfileirar um processo */
/* Recebe a fila e o processo a ser enfileirado */
/* Nao retorna valores */
void enfileira(Fila *fila, Processo processo);

/* Funcao para enfileirar um processo no inicio da fila */
/* Recebe a fila e o processo a ser enfileirado */
/* Nao retorna valores */
void enfileira_inicio(Fila *fila, Processo processo);

/* Funcao para desenfileirar um processo */
/* Recebe a fila */
/* Retorna um ponteiro para o processo desenfileirado */
Processo desenfileira(Fila *fila);

/* Funcao para esvaziar uma fila */
/* Recebe a fila a ser esvaziada */
/* Nao retorna valores */
void esvazia_fila(Fila *fila);

/* Funcao para imprimir uma fila */
/* Recebe a fila a ser impressa */
/* Nao retorna valores */
void imprime_fila(Fila *fila);

/* Funcao para imprimir uma fila formatada */
/* Recebe o nome da fila e a fila a ser impressa */
/* Nao retorna valores */
void imprime_fila_formatada(const char *nome_fila, Fila *fila);

/* Funcao para imprimir todas as filas */
/* Recebe as filas a serem impressas */
/* Nao retorna valores */
void imprime_filas(Fila *fila_alta_prioridade, Fila *fila_baixa_prioridade, Fila *fila_disco, Fila *fila_fita, Fila *fila_impressora);

#endif /* FILA_H */