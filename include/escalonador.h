#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "fila.h"
#include "processo.h"

/* Função para processar a fila de prioridade */
void processa_fila_prioridade(Fila *fila_prioridade, ListaFila *lista_filas, ListaProcessos *lista_processos, int instante_atual, int *processos_finalizados);

/* Função para processar a fila de I/O */
void processa_fila_io(Fila *fila_io, Fila *fila_destino, const char *prioridade, int instante_atual, int *processos_finalizados);

/* Função que simula o escalonador */
/* Não retorna valores */
void escalonador(ListaFila *lista_filas, ListaProcessos *lista_processos);

/* Função que verifica se todas as filas estão vazias */
/* Retorna 1 caso todas as filas estiverem vazias; Retorna 0 caso contrário */
int todas_filas_vazias(ListaFila todas_filas);

/* Função que verifica e enfileira novos processos com instante de chegada atual */
/* Não retorna valores */
void verifica_novos_processos(Fila *fila, ListaProcessos *lista_processos, int instante_atual);

/* Função que envia um processo para uma das filas de IO */
/* Não retorna valores */
void envia_para_io(Processo *processo, ListaFila *lista_filas);

/* Função para atualizar o tempo de turnaround de um processo */
/* Não retorna valores */
void atualiza_turnaround(Processo *processo_atual, int instante_fim);

#endif /* ESCALONADOR_H */
