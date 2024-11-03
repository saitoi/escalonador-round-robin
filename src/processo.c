#include <stdio.h>
#include <stdlib.h>

#include "../include/processo.h"
#include "../include/auxiliar.h"

Processo *aloca_processo(void) {
    Processo *processo = (Processo *) malloc(sizeof(Processo));
    
    if (processo == NULL) {
        tratar_erro_alocacao("Erro na alocação de processo.\n");
        return NULL;
    }

    return processo;
}

Processo *aloca_multiplos_processos(int num_processos) {
    Processo *processos = (Processo *) calloc(num_processos, sizeof(Processo));

    if (processos == NULL) {
        tratar_erro_alocacao("Erro na alocação de múltiplos processos.\n");
        return NULL;
    }

    return processos;
}

OperacaoIO *aloca_operacoes_io(int num_operacoes_io) {
    OperacaoIO *operacoes_io = NULL;

    operacoes_io = (OperacaoIO *) calloc(num_operacoes_io, sizeof(OperacaoIO));

    if (operacoes_io == NULL) {
        tratar_erro_alocacao("Erro na alocação de operação de IO");
        return NULL;
    }

    return operacoes_io;
}

int seleciona_tempo_io(TipoIO tipo_io) {
    switch (tipo_io) {
        case DISCO:
            return TEMPO_DISCO;
        case FITA:
            return TEMPO_FITA;
        default:
            return 0;
    }
}

const char *seleciona_tipo_io(TipoIO tipo_io) {
    switch (tipo_io) {
        case DISCO:
            return "Disco";
        case FITA:
            return "Fita";
        default:
            return "Desconhecido";
    }
}

const char *seleciona_status_processo(StatusProcesso status_processo) {
    switch (status_processo) {
        case PRONTO:
            return "PRONTO";
        case EXECUTANDO:
            return "EXECUTANDO";
        case ENTRADA_SAIDA:
            return "ENTRADA E/OU SAIDA";
        default:
            return "DESCONHECIDO";
    }
}

ListaProcessos criar_lista_processos_aleatorios(void) {
    ListaProcessos lista;
    lista.quantidade = MAXIMO_PROCESSOS;
    lista.processos = (Processo *)calloc(MAXIMO_PROCESSOS, sizeof(Processo));
    
    if (!lista.processos) {
        tratar_erro_alocacao("Erro ao alocar memória para a lista de processos.\n");
    }

    for (int pid = 0; pid < MAXIMO_PROCESSOS; pid++) {
        lista.processos[pid] = criar_processo_aleatorio(pid);
    }

    return lista;
}

Processo criar_processo_aleatorio(int pid) {
    Processo processo;

    processo.pid = pid;
    processo.instante_chegada = gerar_dado_aleatorio(0, TEMPO_MAX_CHEGADA);
    processo.tempo_cpu = gerar_dado_aleatorio(TEMPO_MIN_CPU, TEMPO_MAX_CPU);
    processo.tempo_cpu_restante = processo.tempo_cpu;
    processo.tempo_quantum_restante = 0;
    processo.tempo_cpu_atual = 0;
    processo.status_processo = PRONTO;

    processo.num_operacoes_io = gerar_dado_aleatorio(0, QUANTIDADE_TIPOS_IO);
    if (processo.num_operacoes_io > 0) {
        processo.operacao_io_atual = 0;
        processo.operacoes_io = aloca_operacoes_io(processo.num_operacoes_io);
        if (!processo.operacoes_io) {
            tratar_erro_alocacao("Erro ao alocar operações de IO.\n");
        }

        for (int i = 0; i < processo.num_operacoes_io; i++) {
            processo.operacoes_io[i].tipo_io = gerar_dado_aleatorio(0, QUANTIDADE_TIPOS_IO - 1);
            processo.operacoes_io[i].duracao_io = seleciona_tempo_io(processo.operacoes_io[i].tipo_io);
            processo.operacoes_io[i].tempo_inicio = gerar_dado_aleatorio(0, processo.tempo_cpu);
            processo.operacoes_io[i].tempo_restante = processo.operacoes_io[i].duracao_io;
        }

        /* Ordena as operações de IO pelo tempo de início */
        quicksort(processo.operacoes_io, 0, processo.num_operacoes_io - 1);
    } else {
        processo.operacoes_io = NULL;
    }

    return processo;
}


// Pode ter número variável de processos
ListaProcessos criar_lista_processos_csv(const char *nome_arquivo) {
    ListaProcessos lista = {NULL, 0};
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        tratar_erro_alocacao("Erro ao abrir arquivo de entrada.\n");
    }

    Processo *processos = calloc(MAXIMO_PROCESSOS, sizeof(Processo));
    if (!processos) {
        tratar_erro_alocacao("Erro ao alocar memória para processos.\n");
        fclose(arquivo);
    }

    char linha[MAX_LINHA];
    int qtd_processos_lidos = 0;

    /* Ignora a primeira linha (cabeçalho) */
    if (fgets(linha, MAX_LINHA, arquivo) == NULL) {
        // Arquivo vazio ou erro na leitura
        fclose(arquivo);
        free(processos);
        return lista;
    }

    while (fgets(linha, MAX_LINHA, arquivo) && qtd_processos_lidos < MAXIMO_PROCESSOS) {
        Processo *processo = &processos[qtd_processos_lidos];
        if (!parse_linha_csv(linha, processo)) {
            continue; /* Linha inválida, vai para a próxima */
        }
        qtd_processos_lidos++;
    }

    fclose(arquivo);

    if (qtd_processos_lidos == 0) {
        // Nenhum processo válido foi lido
        free(processos);
        return lista;
    }

    /* Redimensiona o array para a quantidade real de processos lidos */
    Processo *processos_redimensionados = realloc(processos, qtd_processos_lidos * sizeof(Processo));
    if (!processos_redimensionados) {
        tratar_erro_alocacao("Erro ao redimensionar array de processos.\n");
        free(processos);
        return lista;
    }

    lista.processos = processos_redimensionados;
    lista.quantidade = qtd_processos_lidos;

    return lista;
}

// TODO: Remover essa função
/*Processo *configurar_processo_usuario(void) {*/
/*    Processo *processo = aloca_processo();*/
/*    int i = 0;*/
/**/
/*    processo->pid = 0;*/
/**/
/*    valida_entrada_inteiro(*/
/*        "1. Instante de chegada (0 - 4)",*/
/*        (int *)&(processo->instante_chegada),*/
/*        TEMPO_MIN_CHEGADA,*/
/*        TEMPO_MAX_CHEGADA*/
/*    );*/
/**/
/*    valida_entrada_inteiro(*/
/*        "2. Tempo de CPU (5 - 10)",*/
/*        (int *)&(processo->tempo_cpu),*/
/*        TEMPO_MIN_CPU,*/
/*        TEMPO_MAX_CPU*/
/*    );*/
/**/
/*    valida_entrada_inteiro(*/
/*        "2. Escolha a quantidade de operações de I/O (0 - )",*/
/*        (int *)&(processo->num_operacoes_io),*/
/*        0,*/
/*        QUANTIDADE_TIPOS_IO*/
/*    );*/
/**/
/*    if (processo->num_operacoes_io == 0)*/
/*        return processo;*/
/**/
/*    processo->operacao_io_atual = 0;*/
/*    processo->operacoes_io = aloca_operacoes_io(processo->num_operacoes_io);*/
/**/
/*    for (i = 0; i < processo->num_operacoes_io; ++i) {*/
/*        valida_entrada_inteiro(*/
/*            "2.1. Escolha o tipo de operação de I/O (0: DISCO, 1: FITA)",*/
/*            (int *)&(processo->operacoes_io[i].tipo_io),*/
/*            0,*/
/*            QUANTIDADE_TIPOS_IO*/
/*        );*/
/**/
/*        processo->operacoes_io[i].duracao_io = seleciona_tempo_io(processo->operacoes_io[i].tipo_io);*/
/**/
/*        valida_entrada_inteiro(*/
/*            "2.2. Escolha o instante de início da operação de I/O (0 - 4)",*/
/*            (int *)&(processo->operacoes_io[i].tempo_inicio),*/
/*            0,*/
/*            QUANTIDADE_TIPOS_IO + 1*/
/*        );*/
/**/
/*        processo->operacoes_io[i].tempo_restante = processo->operacoes_io[i].duracao_io;*/
/*    }*/
/**/
/*    // TODO: Trocar operação de quicksort*/
/*    quicksort(processo->operacoes_io, 0, processo->num_operacoes_io - 1);*/
/**/
/*    return processo;*/
/*}*/

// TODO: Mudar essa função para se adequar

// Processo *inicializa_processos(int qtd_processos) {
//     Processo *processos = aloca_multiplos_processos(qtd_processos);
//     int i = 0;

//     for (i = 0; i < qtd_processos; i++)
//         processos[i] = *criar_processo_aleatorio(i);

//     return processos;
// }

void executa_processo(Processo *processo) {
    printf(" P%d executou por 1 u.t.\n", processo->pid);
    processo->tempo_cpu_restante -= 1;
    processo->tempo_cpu_atual += 1;
    processo->tempo_quantum_restante += 1;
    processo->status_processo = EXECUTANDO;
}

int processo_finalizado(Processo *processo) {
    if (processo->tempo_cpu_restante == 0) {
        printf(" P%d finalizou sua execucao.\n", processo->pid);
        return 1;
    }
    return 0;
}

int tempo_inicio_io(Processo *processo) {
    if (processo->operacoes_io != NULL) {
        if (processo->tempo_cpu_atual == processo->operacoes_io[processo->operacao_io_atual].tempo_inicio)
            return 1;
        else
            return 0;
    }
    return 0;
}

void executa_io(Processo *processo) {
    if (processo->status_processo == EXECUTANDO)
        processo->status_processo = ENTRADA_SAIDA;
    else {
        processo->operacoes_io[processo->operacao_io_atual].tempo_restante -= 1;
        printf(" P%d executou 1 u.t. da sua E/S de %s, faltam %d u.t.\n", processo->pid,
               seleciona_tipo_io(processo->operacoes_io[processo->operacao_io_atual].tipo_io),
               processo->operacoes_io[processo->operacao_io_atual].tempo_restante);
    }
}

int io_finalizada(Processo *processo) {
    if (processo->operacoes_io[processo->operacao_io_atual].tempo_restante < 1) {
        printf(" P%d finalizou sua E/S de %s,", processo->pid,
               seleciona_tipo_io(processo->operacoes_io[processo->operacao_io_atual].tipo_io));
        processo->tempo_quantum_restante = 0;
        processo->operacao_io_atual++;
        return 1;
    }
    return 0;
}

int tempo_quantum_completo(Processo *processo, int quantum) {
    if (processo->tempo_quantum_restante == quantum) {
        processo->tempo_quantum_restante = 0;
        return 1;
    }
    return 0;
}
