#ifndef PROCESSO_H
#define PROCESSO_H

#define MAXIMO_PROCESSOS 5                  /* Numero maximo de processos */
static const int TEMPO_MIN_CPU = 3;         /* Tempo minimo de CPU */
static const int TEMPO_MAX_CPU = 10;        /* Tempo maximo de CPU */
static const int TEMPO_MAX_CHEGADA = 4;     /* Tempo maximo de inicio */

static const int TEMPO_DISCO = 2;           /* Tempo de execucao de disco */
static const int TEMPO_FITA = 3;            /* Tempo de execucao de fita */
static const int TEMPO_IO_PADRAO = -1;      /* Tempo atribuído ao IO caso não existir */

static const int QUANTUM = 3;               /* Quantum de tempo definido externamente */

/* Enumeracao contendo os status do processo */
typedef enum _StatusProcesso {
    PRONTO,
    EXECUTANDO,
    ENTRADA_SAIDA
} StatusProcesso;

/* Enumeracao contendo os tipos de E/S */
typedef enum _TipoIO {
    DISCO,
    FITA,
    QUANTIDADE_TIPOS_IO
} TipoIO;

/* Estrutura para armazenar as operacoes de E/S */
typedef struct _OperacaoIO {
    TipoIO tipo_io;     /* Tipo de E/S */
    int presente;       /* Indica se a operacao de E/S está ou não presente */
    int duracao_io;     /* Tempo de duracao da operacao de E/S */
    int tempo_inicio;   /* Tempo de inicio da operacao de E/S */
    int tempo_restante; /* Tempo restante para finalizar a operacao de E/S */
} OperacaoIO;

/* Estrutura para representar os processos */
typedef struct _Processo {
    OperacaoIO operacoes_io[2];         /* Vetor de operacoes de E/S */
    StatusProcesso status_processo;     /* Status do processo */
    int pid;                            /* Identificador do processo */
    int instante_chegada;               /* Tempo de inicio do processo */
    int tempo_cpu;                      /* Tempo de CPU do processo */
    int tempo_fim;                      /* Tempo de fim do processo */
    int tempo_turnaround;               /* Tempo de turnaround do processo */
    int tempo_quantum_restante;         /* Tempo de quantum restante do processo */
    int tempo_cpu_restante;             /* Tempo de CPU restante do processo */
    int tempo_cpu_atual;                /* Tempo de CPU atual do processo */
    int num_operacoes_io;               /* Quantidade de operacoes de E/S */
    int tipo_io_atual;                  /* Indice da operacao de E/S atual */
} Processo;

/* Estrutura para englobar todos os processos */
typedef struct _ListaProcessos {
    Processo processos[MAXIMO_PROCESSOS];   /* Array de todos processos inicializados */
    int quantidade;                         /* Quantidade de processos ativos */
} ListaProcessos;

/* Funcao para imprimir uma solicitacao ao usuario na tela e aguardar a resposta */
void trim_novalinha(char *str);

/* Funcao para selecionar o status de um processo */
/* Recebe o status do processo */
/* Retorna uma string contendo o status do processo */
const char *seleciona_status_processo(StatusProcesso status_processo);

/* Funcao para criar uma lista com quantidade máxima de processos aleatórios */
/* Retorna uma lista de processos aleatorios */
ListaProcessos criar_lista_processos_aleatorios(void);

/* Funcao para criar um processo com seed aleatória */
/* Recebe o pid do processo */
/* Retorna um ponteiro para os processos criados */
Processo criar_processo_aleatorio(void);

/* Funcao para criar um processo a partir da leitura do CSV */
/* Recebe o nome do arquivo para leitura dos dados */
/* Retorna um ponteiro para os processos criados */
ListaProcessos criar_lista_processos_csv(const char *nome_arquivo);

/* Recebe o nome do arquivo para leitura dos dados */
/* Atualiza os dados do processo com os extraídos do CSV */
/* Retorna 1 caso a leitura foi um sucesso e 0 caso contrário */
int parse_linha_csv(char *linha, Processo *processo);

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
int tempo_quantum_completo(Processo *processo);

#endif /* PROCESSO_H */
