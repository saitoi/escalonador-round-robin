#include <stdio.h>
#include <stdlib.h>

#include "../include/auxiliar.h"
#include "../include/processo.h"

void tratar_erro_alocacao(const char *mensagem) {
    fprintf(stderr, "%s\n", mensagem);
    exit(1);
}

void valida_entrada_inteiro(const char *mensagem, int *variavel, int min, int max) {
    int opcao_valida = 1;
    int valor;
    char ch;

    do {
        printf("%s (atual: %d): ", mensagem, *variavel);
        if (scanf("%d", &valor) != 1) {
            printf("Caractere inválido.");
            while ((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }

        if (valor < min || valor > max) {
            printf("Entrada inválida, escolha um valor dentro do intervalo.\n");
            while ((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }

        *variavel = valor;
        opcao_valida = 1;

    } while(!opcao_valida);
}

int gerar_dado_aleatorio(int min, int max) {
    return min + rand() % (max - min + 1);
}

int parse_linha_csv(char *linha, Processo *processo) {
    char *token;
    int idx = 0;

    memset(processo, 0, sizeof(Processo));

    token = strtok(linha, ",");
    while (token != NULL) {
        switch (idx) {
            case 0:
                processo->pid = atoi(token);
                break;
            case 1:
                processo->instante_chegada = atoi(token);
                break;
            case 2:
                processo->tempo_cpu = atoi(token);
                processo->tempo_cpu_restante = processo->tempo_cpu;
                processo->tempo_quantum_restante = 0;
                processo->tempo_cpu_atual = 0;
                processo->status_processo = PRONTO;
                break;
            case 3:
                processo->num_operacoes_io = atoi(token);
                if (processo->num_operacoes_io > 0) {
                    processo->operacoes_io = aloca_operacoes_io(processo->num_operacoes_io);
                    if (!processo->operacoes_io) {
                        tratar_erro_alocacao("Erro ao alocar operações de IO.\n");
                        return 0;
                    }
                    processo->operacao_io_atual = 0;
                }
                break;
            default:
                if (processo->num_operacoes_io > 0) {
                    int io_idx = (idx - 4) / 2;
                    int io_field = (idx - 4) % 2;
                    if (io_idx < processo->num_operacoes_io) {
                        if (io_field == 0) {
                            processo->operacoes_io[io_idx].tipo_io = atoi(token);
                            processo->operacoes_io[io_idx].duracao_io = seleciona_tempo_io(processo->operacoes_io[io_idx].tipo_io);
                            processo->operacoes_io[io_idx].tempo_restante = processo->operacoes_io[io_idx].duracao_io;
                        } else {
                            processo->operacoes_io[io_idx].tempo_inicio = atoi(token);
                        }
                    }
                }
                break;
        }
        idx++;
        token = strtok(NULL, ",");
    }

    if (idx < 4) {
        return 0;
    }

    if (processo->num_operacoes_io > 0) {
        quicksort_operacoes_io(processo->operacoes_io, 0, processo->num_operacoes_io - 1);
    }

    return 1;
}

void quicksort(OperacaoIO *operacoes_io, int inicio, int fim) {
    int pos_pivo = 0;

    if (fim > inicio) {
        pos_pivo = particiona(operacoes_io, inicio, fim);
        quicksort(operacoes_io, inicio, pos_pivo - 1);
        quicksort(operacoes_io, pos_pivo + 1, fim);
    }
}

int particiona(OperacaoIO *operacoes_io, int inicio, int fim) {
    int pos_pivo = 0;
    OperacaoIO pivo;
    int i = inicio, j = fim;

    pos_pivo = rand() % (fim + 1 - inicio) + inicio;
    pivo = operacoes_io[pos_pivo];

    operacoes_io[pos_pivo] = operacoes_io[inicio];
    operacoes_io[inicio] = pivo;

    while (i < j) {
        while (i < fim && operacoes_io[i].tempo_inicio <= pivo.tempo_inicio) i++;
        while (j > inicio && operacoes_io[j].tempo_inicio > pivo.tempo_inicio) j--;
        if (i < j) troca(&operacoes_io[i], &operacoes_io[j]);
    }

    operacoes_io[inicio] = operacoes_io[j];
    operacoes_io[j] = pivo;
    return j;
}

void troca(OperacaoIO *a, OperacaoIO *b) {
    OperacaoIO temp = *b;
    *b = *a;
    *a = temp;
}
