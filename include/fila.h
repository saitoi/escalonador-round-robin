#ifndef FILA_H
#define FILA_H

#include <stdio.h>
#include <stdlib.h>

#include "processo.h"

/* Definição do número de filas */
#define NUM_FILAS 4 // Exemplo: FILA_ALTA_PRIORIDADE, FILA_BAIXA_PRIORIDADE, FILA_DISCO, FILA_FITA

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

/* Função para alocar o espaço de uma fila */
/* Não recebe parâmetros */
/* Retorna um ponteiro para a fila alocada */
Fila *aloca_fila(void);

/* Função para alocar o espaço de um nó */
/* Não recebe parâmetros */
/* Retorna um ponteiro para o nó alocado */
No *aloca_no(void);

/* Função para inicializar uma fila */
/* Recebe a fila a ser inicializada */
/* Não retorna valores */
void inicializa_fila(Fila *fila);

/* Função para inicializar todas as filas em uma lista */
/* Recebe a lista de filas a ser inicializada */
/* Não retorna valores */
void inicializa_lista_filas(ListaFila *lista);

/* ************* FUNÇÕES DE MANIPULAÇÃO DE FILA ************* */

/* Função para verificar se a fila está vazia */
/* Recebe a fila a ser verificada */
/* Retorna 1 caso a fila esteja vazia, 0 caso contrário */
int fila_vazia(Fila *fila);

/* Função para enfileirar um processo no final da fila */
/* Recebe a fila e um ponteiro para o processo a ser enfileirado */
/* Não retorna valores */
void enfileira(Fila *fila, Processo *processo);

/* Função para enfileirar um processo no início da fila */
/* Recebe a fila e um ponteiro para o processo a ser enfileirado */
/* Não retorna valores */
void enfileira_inicio(Fila *fila, Processo *processo);

/* Função para desenfileirar um processo da fila */
/* Recebe a fila */
/* Retorna um ponteiro para o processo desenfileirado */
Processo *desenfileira(Fila *fila);

/* Função para esvaziar uma fila e liberar a memória alocada para os nós */
/* Recebe a fila a ser esvaziada */
/* Não retorna valores */
void esvazia_fila(Fila *fila);

/* Função para imprimir uma fila formatada */
/* Recebe o nome da fila e a fila a ser impressa */
/* Não retorna valores */
void imprime_fila_formatada(const char *nome_fila, Fila *fila);

#endif /* FILA_H */
