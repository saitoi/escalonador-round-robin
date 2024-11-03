#ifndef PROCESSO_H
#define PROCESSO_H

#define TEMPO_MIN_CPU 5     /* Tempo minimo de CPU */
#define TEMPO_MAX_CPU 10    /* Tempo maximo de CPU */
#define TEMPO_MIN_CHEGADA 0 /* Tempo maximo de inicio */
#define TEMPO_MAX_CHEGADA 4 /* Tempo maximo de inicio */

#define MAXIMO_PROCESSOS 4      /* Numero maximo de processos */
#define TEMPO_DISCO 2           /* Tempo de execucao de disco */
#define TEMPO_FITA 3            /* Tempo de execucao de fita */
#define QUANTIDADE_TIPOS_IO 2   /* Quantidade de tipos de E/S */

/* Enumeracao contendo os tipos de E/S */
typedef enum _TipoIO {
    DISCO,
    FITA
} TipoIO;

/* Estrutura para armazenar as operacoes de E/S */
typedef struct _OperacaoIO {
    TipoIO tipo_io;     /* Tipo de E/S */
    int duracao_io;     /* Tempo de duracao da operacao de E/S */
    int tempo_inicio;   /* Tempo de inicio da operacao de E/S */
    int tempo_restante; /* Tempo restante para finalizar a operacao de E/S */
} OperacaoIO;

/* Enumeracao contendo os status do processo */
typedef enum _StatusProcesso {
    PRONTO,
    EXECUTANDO,
    ENTRADA_SAIDA
} StatusProcesso;

/* Estrutura para armazenar os processos */
typedef struct _Processo {
    OperacaoIO *operacoes_io;   /* Vetor de operacoes de E/S */
    StatusProcesso status_processo; /* Status do processo */
    int pid;                    /* Identificador do processo */
    int instante_chegada;       /* Tempo de inicio do processo */
    int tempo_cpu;              /* Tempo de CPU do processo */
    int tempo_fim;              /* Tempo de fim do processo */
    int tempo_turnaround;       /* Tempo de turnaround do processo */
    int tempo_quantum_restante; /* Tempo de quantum restante do processo */
    int tempo_cpu_restante;     /* Tempo de CPU restante do processo */
    int tempo_cpu_atual;        /* Tempo de CPU atual do processo */
    int num_operacoes_io;       /* Quantidade de operacoes de E/S */
    int operacao_io_atual;      /* Indice da operacao de E/S atual */
} Processo;

/* Estrutura para englobar todos os processos */
typedef struct _ListaProcessos {
    Processo *processos;        /* Array de todos processos inicializados */
    int quantidade;             /* Quantidade de processos ativos */
} ListaProcessos;

/* Funcao para alocar o espaco de um processo */
/* Nao recebe parametros */
/* Retorna um ponteiro para o processo alocado */
Processo *aloca_processo(void);

/* Funcao para alocar o espaco de um vetor de processos */
/* Recebe a quantidade de processos a serem alocados */
/* Retorna um ponteiro para o vetor de processos alocado */
Processo *aloca_multiplos_processos(int num_processos);

/* Funcao para alocar o espaco de um vetor de operacoes de E/S */
/* Recebe a quantidade de operacoes de E/S a serem alocadas */
/* Retorna um ponteiro para o vetor de operacoes de E/S alocado */
OperacaoIO *aloca_operacoes_io(int num_operacoes_io);

/* Funcao para selecionar o tempo de uma operacao de E/S */
/* Recebe o tipo de E/S */
/* Retorna o tempo da operacao de E/S */
int seleciona_tempo_io(TipoIO tipo_io);

/* Funcao para selecionar o tipo de uma operacao de E/S */
/* Recebe o tipo de E/S */
/* Retorna uma string contendo o tipo de E/S */
const char *seleciona_tipo_io(TipoIO tipo_io);

/* Funcao para selecionar o status de um processo */
/* Recebe o status do processo */
/* Retorna uma string contendo o status do processo */
const char *seleciona_status_processo(StatusProcesso status_processo);

/* Funcao para criar um processo com seed aleatória */
/* Recebe o pid do processo */
/* Retorna um ponteiro para os processos criados */
Processo *criar_processos_aleatorios();

/* Funcao para criar uma lista com quantidade máxima de processos aleatórios */
/* Retorna uma lista de processos aleatorios */
ListaProcessos criar_lista_processos_aleatorios(void);

/* Funcao para criar um processo com seed aleatória */
/* Recebe o pid do processo */
/* Retorna um ponteiro para os processos criados */
Processo criar_processo_aleatorio(int pid);


/* Funcao para criar um processo a partir da leitura do CSV */
/* Recebe o nome do arquivo para leitura dos dados */
/* Retorna um ponteiro para os processos criados */
ListaProcessos criar_lista_processos_csv(const char *nome_arquivo);


/* Funcao para inicializar os processos */
/* Recebe a quantidade de processos a serem inicializados */
/* Retorna um ponteiro para o vetor de processos inicializados */
// Processo *inicializa_processos(int qtd_processos);

// TODO: Remover essa declaracao, função e removê-la do menu
Processo *configurar_processo_usuario(void);

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
int tempo_inicio_io(Processo *processo);

/* Funcao para executar uma operacao de E/S */
/* Recebe o processo a ser verificado */
/* Nao retorna valores */
void executa_io(Processo *processo);

/* Funcao para verificar se uma operacao de E/S finalizou sua execucao */
/* Recebe o processo a ser verificado */
/* Retorna 1 caso a operacao de E/S tenha finalizado, 0 caso contrario */
int io_finalizada(Processo *processo);

/* Funcao para verificar se um processo atingiu o tempo de quantum */
/* Recebe o processo a ser verificado e o tempo de quantum */
/* Retorna 1 caso o processo tenha atingido o tempo de quantum, 0 caso contrario */
int tempo_quantum_completo(Processo *processo, int quantum);

#endif /* PROCESSO_H */
