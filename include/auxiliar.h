#ifndef AUXILIAR_H
#define AUXILIAR_H

static const int MAX_LINHA = 256;

#include "processo.h"

/* Funcao para imprimir uma solicitacao ao usuario na tela e aguardar a resposta */
/* Valida a entrada inserida pelo usuário */
/* Não retorna valores */
void enviar_mensagem_erro(const char *mensagem);

void valida_entrada_inteiro(int valor, int min, int max, int *opcao_valida);

/* Funcao para imprimir uma solicitacao ao usuario na tela e aguardar a resposta */
/* Valida a entrada inserida pelo usuário */
/* Não retorna valores */
void obter_entrada_inteiro(const char *mensagem, int *variavel, int min, int max);

int converter_validar_int(const char *str, int min, int max, int *opcao_valida);


/* Valida a entrada de caracter inserida pelo usuário */
/* Não retorna valores */
void valida_entrada_char(void);

/* Funcao para gerar um valor aleatório dado dois limites: Um mínimo e outro máximo */
/* Retorna um inteiro */
int gerar_dado_aleatorio(int min, int max);

/* Retorna um inteiro correspondente */
int parse_linha_csv(char *linha, Processo *processo);

/* Funcao para selecionar o tempo de uma operacao de E/S */
/* Recebe o tipo de E/S */
/* Retorna o tempo da operacao de E/S */
int seleciona_tempo_io(TipoIO tipo_io);

/* Funcao para selecionar o tipo de uma operacao de E/S */
/* Recebe o tipo de E/S */
/* Retorna uma string contendo o tipo de E/S */
const char *seleciona_tipo_io(TipoIO tipo_io);

#endif /* AUXILIAR_H */
