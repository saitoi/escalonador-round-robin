#ifndef PROCESSO_H
#define PROCESSO_H

#define TEMPO_DISCO 2           /* Tempo de execucao de disco */
#define TEMPO_FITA 3            /* Tempo de execucao de fita */
#define TEMPO_IMPRESSORA 4      /* Tempo de execucao de impressora */
#define QUANTIDADE_TIPOS_IO 3   /* Quantidade de tipos de E/S */

/* Enumeracao contendo os tipos de E/S */
typedef enum _TipoES {
    DISCO,
    FITA,
    IMPRESSORA
} TipoES;

/* Estrutura para armazenar as operacoes de E/S */
typedef struct _OperacaoES {
    TipoES tipo_es;     /* Tipo de E/S */
    int duracao_es;     /* Tempo de duracao da operacao de E/S */
    int tempo_inicio;   /* Tempo de inicio da operacao de E/S */
    int tempo_restante; /* Tempo restante para finalizar a operacao de E/S */
} OperacaoES;

/* Enumeracao contendo os status do processo */
typedef enum _StatusProcesso {
    PRONTO,
    EXECUTANDO,
    ENTRADA_SAIDA
} StatusProcesso;

/* Estrutura para armazenar os processos */
typedef struct _Processo {
    int pid; /* Identificador do processo */
    int tempo_inicio; /* Tempo de inicio do processo */
    int tempo_cpu; /* Tempo de CPU do processo */
    int tempo_fim; /* Tempo de fim do processo */
    int tempo_turnaround; /* Tempo de turnaround do processo */
    int tempo_quantum_restante; /* Tempo de quantum restante do processo */
    int tempo_cpu_restante; /* Tempo de CPU restante do processo */
    int tempo_cpu_atual; /* Tempo de CPU atual do processo */

    OperacaoES *operacoes_es; /* Vetor de operacoes de E/S */
    int num_operacoes_es; /* Quantidade de operacoes de E/S */
    int operacao_es_atual; /* Indice da operacao de E/S atual */
    StatusProcesso status_processo; /* Status do processo */
} Processo;

/* Funcao para alocar o espaco de um processo */
/* Nao recebe parametros */
/* Retorna um ponteiro para o processo alocado */
Processo *aloca_processo(void);

/* Funcao para alocar o espaco de um vetor de processos */
/* Recebe a quantidade de processos a serem alocados */
/* Retorna um ponteiro para o vetor de processos alocado */
Processo *aloca_processos(int num_processos);

/* Funcao para alocar o espaco de um vetor de operacoes de E/S */
/* Recebe a quantidade de operacoes de E/S a serem alocadas */
/* Retorna um ponteiro para o vetor de operacoes de E/S alocado */
OperacaoES *aloca_operacoes_es(int num_operacoes_es);

/* Funcao para selecionar o tempo de uma operacao de E/S */
/* Recebe o tipo de E/S */
/* Retorna o tempo da operacao de E/S */
int seleciona_tempo_es(TipoES tipo_es);

/* Funcao para selecionar o tipo de uma operacao de E/S */
/* Recebe o tipo de E/S */
/* Retorna uma string contendo o tipo de E/S */
const char *seleciona_tipo_es(TipoES tipo_es);

/* Funcao para selecionar o status de um processo */
/* Recebe o status do processo */
/* Retorna uma string contendo o status do processo */
const char *seleciona_status_processo(StatusProcesso status_processo);

/* Funcao para criar um processo */
/* Recebe o pid do processo */
/* Retorna um ponteiro para o processo criado */
Processo *cria_processo(int pid);

/* Funcao para inicializar os processos */
/* Recebe a quantidade de processos a serem inicializados */
/* Retorna um ponteiro para o vetor de processos inicializados */
Processo *inicializa_processos(int qtd_processos);

/* Funcao para executar um processo */
/* Recebe o processo a ser executado */
/* Nao retorna valores */
void executa_processo(Processo *processo);

/* Funcao para verificar se um processo finalizou sua execucao */
/* Recebe o processo a ser verificado */
/* Retorna 1 caso o processo tenha finalizado, 0 caso contrario */
int processo_finalizado(Processo *processo);

/* Funcao para verificar se um processo deve iniciar uma operacao de E/S */
/* Recebe o processo a ser verificado */
/* Retorna 1 caso o processo deva iniciar uma operacao de E/S, 0 caso contrario */
int tempo_inicio_es(Processo *processo);

/* Funcao para executar uma operacao de E/S */
/* Recebe o processo a ser verificado */
/* Nao retorna valores */
void executa_es(Processo *processo);

/* Funcao para verificar se uma operacao de E/S finalizou sua execucao */
/* Recebe o processo a ser verificado */
/* Retorna 1 caso a operacao de E/S tenha finalizado, 0 caso contrario */
int es_finalizada(Processo *processo);

/* Funcao para verificar se um processo atingiu o tempo de quantum */
/* Recebe o processo a ser verificado e o tempo de quantum */
/* Retorna 1 caso o processo tenha atingido o tempo de quantum, 0 caso contrario */
int tempo_quantum_completo(Processo *processo, int quantum);

#endif /* PROCESSO_H */
