#ifndef AUXILIAR_H
#define AUXILIAR_H

#include "processo.h"

// TODO: Mudar esse nome
//
/* Funcao para imprimir uma solicitacao ao usuario na tela e aguardar a resposta */
/* Valida a entrada inserida pelo usuário */
/* Não retorna valores */
void tratar_erro_alocacao(const char *mensagem);

/* Funcao para imprimir uma solicitacao ao usuario na tela e aguardar a resposta */
/* Valida a entrada inserida pelo usuário */
/* Não retorna valores */
void valida_entrada_inteiro(const char *mensagem, int *variavel, int min, int max);

/* Funcao para realizar a ordenacao rapida */
/* Recebe um vetor de operacoes de E/S, o inicio e o fim do vetor */
/* Nao retorna valores */
void quicksort(OperacaoIO *operacoes_io, int inicio, int fim);

/* Funcao para particionar o vetor */
/* Recebe um vetor de operacoes de E/S, o inicio e o fim do vetor */
/* Retorna a posicao do pivo */
int particiona(OperacaoIO *operacoes_io, int inicio, int fim);

/* Funcao para trocar dois elementos de posicao */
/* Recebe dois ponteiros para operacoes de E/S */
/* Nao retorna valores */
void troca(OperacaoIO *a, OperacaoIO *b);

/* Funcao para controle de erros de alocacao */
/* Recebe um ponteiro */
/* Nao retorna valores */
// void controla_erro_alocacao(void *ponteiro);

#endif /* AUXILIAR_H */
