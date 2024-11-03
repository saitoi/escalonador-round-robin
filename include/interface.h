#ifndef INTERFACE_H
#define INTERFACE_H

/* Funcao que simula o escalonador */
/* Recebe os processos a serem executados e as filas de alta prioridade, de baixa prioridade, de disco e de fita */
/* Nao retorna valores */
void processa_menu(Fila *fila_alta_prioridade, Fila *fila_baixa_prioridade, Fila *fila_disco, Fila *fila_fita);

/* Funcao que imprime o instante atual */
/* Nao retorna valores */
void imprime_instante(int tempo_atual);

/* Funcao que imprime o menu para a seleção do usuário */
/* Nao retorna valores */
void imprime_menu(void);

/* Funcao que imprime interface gráfica da fila */
/* Recebe a fila com os processos  */
/* Nao retorna valores */
void imprime_todas_filas(Fila *fila_alta_prioridade, Fila *fila_baixa_prioridade, Fila *fila_disco, Fila *fila_fita);

/* Funcao que imprime interface gráfica da fila */
/* Recebe a fila com os processos  */
/* Nao retorna valores */
void imprime_fila(const char *nome_fila, Fila *fila);

#endif /* INTERFACE_H */
