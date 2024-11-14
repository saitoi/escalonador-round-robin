#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "fila.h"
#include "processo.h"

/* Função para processar a fila de prioridade */
void processa_fila_prioridade(Fila *fila_prioridade, ListaFila *lista_filas, ListaProcessos *lista_processos, int instante_atual, int *processos_finalizados);

/* Função para processar a fila de I/O */
void processa_fila_io(Fila *fila_io, Fila *fila_destino, const char *prioridade, int instante_atual, int *processos_finalizados);

/* Função que simula o escalonador */
/* Recebe as filas e a lista de processos a serem executados */
/* Não retorna valores */
void escalonador(ListaFila *lista_filas, ListaProcessos *lista_processos);

/* Função que verifica se todas as filas estão vazias */
/* Recebe a lista de todas as filas */
/* Retorna 1 caso todas as filas estejam vazias, 0 caso contrário */
int todas_filas_vazias(ListaFila todas_filas);

/* Função que verifica e enfileira novos processos que chegaram */
/* Recebe a fila de alta prioridade, a lista de processos e o instante atual */
/* Não retorna valores */
void verifica_novos_processos(Fila *fila, ListaProcessos *lista_processos, int instante_atual);

/* Função que imprime a turnaround dos processos */
/* Recebe a lista de processos cujos turnarounds serão impressos */
/* Não retorna valores */
void imprime_turnaround_processos(ListaProcessos lista_processos);

/* Função que imprime os tempos de turnaround */
/* Recebe um array de processos */
/* Não retorna valores */
void imprime_tempos_turnaround(Processo *processos);

/* Função que envia um processo para uma das filas de E/S */
/* Recebe o processo a ser enviado e a lista de filas */
/* Não retorna valores */
void envia_para_io(Processo *processo, ListaFila *lista_filas);

/* Função que atualiza o tempo de turnaround de um processo */
/* Recebe o processo cujo tempo de turnaround será atualizado e o instante de fim */
/* Não retorna valores */
void atualiza_turnaround(Processo *processo_atual, int instante_fim);

#endif /* ESCALONADOR_H */
