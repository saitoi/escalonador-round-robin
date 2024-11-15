#ifndef UTILITARIOS_H
#define UTILITARIOS_H

static const int MAX_LINHA = 256;

#include "processo.h"

/* Funcao para exibir uma mensagem de erro e interromper execução do programa */
/* Não retorna valores */
void enviar_mensagem_erro(const char *mensagem);

/* Funcao para verificar se o valor escolhido está contido no intervalo desejado */
/* Não retorna valores */
void valida_entrada_inteiro(int valor, int min, int max, int *opcao_valida);

/* Engloba a função anterior e solicita entrada do usuário */
/* Não retorna valores */
void obter_entrada_inteiro(const char *mensagem, int *variavel, int min, int max);

/* Função para converter as entradas do CSV e validar o resultado usando `valida_entrada_inteiro` */
/* Retorna o inteiro se a validação foi um sucesso; Retorna -1 caso contrário. */
int converter_validar_int(const char *str, int min, int max, int *opcao_valida);

/* Função para solicitar e validar a entrada de caracter inserida pelo usuário */
/* Não retorna valores */
void valida_entrada_char(void);

/* Funcao para gerar um valor aleatório dado dois limites: Um mínimo e outro máximo */
/* Retorna um inteiro */
int gerar_dado_aleatorio(int min, int max);

/* Função para  */
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
