#ifndef FILA_H
#define FILA_H

#include <stdio.h>
#include <stdlib.h>

#include "processo.h"

/* Número de filas */
#define NUM_FILAS 4

/* Estrutura para armazenar um nó */
typedef struct _No {
    Processo *processo; /* Ponteiro para o processo armazenado no nó */
    struct _No *prox;   /* Ponteiro para o próximo nó */
} No;

/* Estrutura para armazenar uma fila */
typedef struct _Fila {
    No *inicio; /* Ponteiro para o início da fila */
    No *fim;    /* Ponteiro para o fim da fila */
} Fila;

/* Enumeração para os tipos de filas */
typedef enum _TipoFila {
    FILA_ALTA_PRIORIDADE,
    FILA_BAIXA_PRIORIDADE,
    FILA_DISCO,
    FILA_FITA
} TipoFila;

/* Estrutura para armazenar uma lista de filas */
typedef struct _ListaFila {
    Fila filas[NUM_FILAS];
} ListaFila;

/* ************* FUNÇÕES DE ALOCACAO E INICIALIZACAO ************* */

/* Função para inicializar o quantum restante dos processos para zero */
/* Não retorna valores */
void inicializa_quantum(Fila *fila);

/* Função para alocar o espaço de um nó */
/* Retorna um ponteiro para o nó alocado */
No *aloca_no(void);

/* Função para inicializar uma fila */
/* Não retorna valores */
void inicializa_fila(Fila *fila);

/* Função para inicializar todas as filas em uma lista */
/* Não retorna valores */
void inicializa_lista_filas(ListaFila *lista);

/* ************* FUNÇÕES DE MANIPULAÇÃO DE FILA ************* */

/* Função para verificar se a fila está vazia */
/* Retorna 1 caso a fila esteja vazia, 0 caso contrário */
int fila_vazia(Fila *fila);

/* Função para enfileirar um processo no final da fila */
/* Não retorna valores */
void enfileira(Fila *fila, Processo *processo);

/* Função para enfileirar um processo no início da fila */
/* Não retorna valores */
void enfileira_inicio(Fila *fila, Processo *processo);

/* Função para desenfileirar um processo da fila */
/* Retorna um ponteiro para o processo desenfileirado */
Processo *desenfileira(Fila *fila);

#endif /* FILA_H */
