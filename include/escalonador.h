#ifndef ESCALONADOR_H
#define ESCALONADOR_H

/* Funcao que simula o escalonador */
/* Recebe os processos a serem executados e as filas de alta prioridade, de baixa prioridade, de disco, de fita e de impressora */
/* Nao retorna valores */
void escalonador(Processo *processos,
    Fila *fila_alta_prioridade, Fila *fila_baixa_prioridade, Fila *fila_disco, Fila *fila_fita, Fila *fila_impressora);

/* Funcao que imprime a informacao dos processos */
/* Recebe os processos a serem impressos */
/* Nao retorna valores */
void imprime_informacao_processos(Processo *processos);

/* Funcao que imprime a informacao dos processos */
/* Recebe os processos a serem impressos */
/* Nao retorna valores */
void verifica_novos_processos(Processo *processos, int tempo_atual, Fila *fila);

/* Funcao que imprime o tempo de turnaround */
/* Recebe os processos cujos tempos de turnaround serao impressos */
/* Nao retorna valores */
void imprime_tempos_turnaround(Processo *processos);

/* Funcao que verifica se ha processos nas filas de alta prioridade e de baixa prioridade */
/* Recebe as filas de alta prioridade e de baixa prioridade */
/* Retorna 1 caso haja processos em uma ou nas duas filas, 0 caso contrario */
int verifica_processador(Fila *fila_alta_prioridade, Fila *fila_baixa_prioridade);

/* Funcao que verifica se ha processos nas filas de disco, de fita e de impressora */
/* Recebe as filas de disco, de fita e de impressora */
/* Retorna 1 caso haja processos em uma ou nas tres filas, 0 caso contrario */
int verifica_es(Fila *fila_disco, Fila *fila_fita, Fila *fila_impressora);

/* Funcao que envia um processo para uma das filas de E/S */
/* Recebe o processo a ser enviado e as filas de disco, de fita e de impressora */
/* Nao retorna valores */
void envia_para_es(Processo processo, Fila *fila_disco, Fila *fila_fita, Fila *fila_impressora);

/* Funcao que atualiza o tempo de turnaround de um processo */
/* Recebe o processo cujo tempo de turnaround sera atualizado, o tempo de turnaround e os processos */
/* Nao retorna valores */
void atualiza_tempo_turnaround(Processo *processo, int turnaround, Processo *processos);

#endif /* ESCALONADOR_H */