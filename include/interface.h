#ifndef INTERFACE_H
#define INTERFACE_H

/* Funcao para atualização da lista de processos baseado na escolha do usuário */
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

/* Função que imprime a informação dos processos */
/* Não retorna valores */
void imprime_tabela_processos(ListaProcessos *lista_processos);

/* Função que imprime a turnaround dos processos */
/* Não retorna valores */
void imprime_turnaround_processos(ListaProcessos lista_processos);

/* Funcao que imprime todas as filas */
/* Nao retorna valores */
void imprime_todas_filas(ListaFila *lista_filas);

/* Funcao que imprime interface gráfica da fila */
/* Nao retorna valores */
void imprime_fila(const char *nome_fila, Fila *fila);

#endif /* INTERFACE_H */
