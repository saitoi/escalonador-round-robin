#ifndef AUXILIAR_H
#define AUXILIAR_H

static const int MAX_LINHA = 256;

#include "processo.h"

// TODO: Mudar esse nome
//
/* Funcao para imprimir uma solicitacao ao usuario na tela e aguardar a resposta */
/* Valida a entrada inserida pelo usuário */
/* Não retorna valores */
void enviar_mensagem_erro(const char *mensagem);

/* Funcao para imprimir uma solicitacao ao usuario na tela e aguardar a resposta */
/* Valida a entrada inserida pelo usuário */
/* Não retorna valores */
void valida_entrada_inteiro(const char *mensagem, int *variavel, int min, int max);

/* Funcao para imprimir uma solicitacao ao usuario na tela e aguardar a resposta */
/* Valida a entrada de caracter inserida pelo usuário */
/* Não retorna valores */
void valida_entrada_char(const char *mensagem, char *variavel);

/* Funcao para gerar um valor aleatório dado dois limites: Um mínimo e outro máximo */
/* Retorna um inteiro */
int gerar_dado_aleatorio(int min, int max);

/* Retorna um inteiro correspondente */
int parse_linha_csv(char *linha, Processo *processo);

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
