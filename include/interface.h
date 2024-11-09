#ifndef INTERFACE_H
#define INTERFACE_H

/* Funcao que simula o escalonador */
/* Recebe os processos a serem executados e as filas de alta prioridade, de baixa prioridade, de disco e de fita */
/* Nao retorna valores */
void processa_menu(ListaProcessos *lista_processos);

/* Funcao que imprime o instante atual */
/* Nao retorna valores */
void imprime_instante(int tempo_atual);

/* Exibe mensagem informando fim do escalonador  */
/* Nao retorna valores */
void imprime_fim_escalonador(void);

/* Funcao que imprime o menu para a seleção do usuário */
/* Nao retorna valores */
void imprime_menu(void);

/* Funcao que imprime interface gráfica da fila */
/* Recebe a fila com os processos  */
/* Nao retorna valores */
void imprime_todas_filas(ListaFila *lista_filas);

/* Funcao que imprime interface gráfica da fila */
/* Recebe a fila com os processos  */
/* Nao retorna valores */
void imprime_fila(const char *nome_fila, Fila *fila);

#endif /* INTERFACE_H */
