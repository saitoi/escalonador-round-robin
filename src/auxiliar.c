#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/auxiliar.h"
#include "../include/processo.h"

void enviar_mensagem_erro(const char *mensagem) {
    fprintf(stderr, "%s\n", mensagem);
    exit(1);
}

void valida_entrada_inteiro(const char *mensagem, int *variavel, int min, int max) {
    int opcao_valida = 0;
    int valor;
    char ch;

    while (!opcao_valida) { 
        printf("%s (%d - %d): ", mensagem, min, max);
        if (scanf("%d", &valor) != 1) {
            printf("Caractere inválido. Tente novamente..\n");
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
     }
}

void valida_entrada_char(const char *mensagem, char *variavel) {
    char ch;
    int opcao_valida = 0;

    while (!opcao_valida) { 
        printf("%s (y/n): ", mensagem);
        if (scanf(" %c", &ch) != 1 || (ch != 'y' && ch != 'n')) {
            printf("Entrada inválida. Tente novamente.\n");
            while ((ch = getchar()) != '\n' && ch != EOF);  // Limpa buffer de entrada
            continue;
        }
        
        *variavel = ch;
        opcao_valida = 1;
    }
}

int gerar_dado_aleatorio(int min, int max) {
    return min + rand() % (max - min + 1);
}

// TODO: Analisar essa função
int parse_linha_csv(char *linha, Processo *processo) {
    char *token;
    int idx = 0;

    memset(processo, 0, sizeof(Processo));

    processo->tipo_io_atual = -1; // Inicializa sem operação de E/S atual
    processo->num_operacoes_io = 0;

    // Inicializa as operações de E/S
    for (int i = 0; i < QUANTIDADE_TIPOS_IO; ++i) {
        processo->operacoes_io[i].tipo_io = i;
        processo->operacoes_io[i].presente = 0;
        processo->operacoes_io[i].duracao_io = seleciona_tempo_io(i);
        processo->operacoes_io[i].tempo_inicio = -1;
        processo->operacoes_io[i].tempo_restante = processo->operacoes_io[i].duracao_io;
    }

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
                // Tempo de início da operação de Disco
                processo->operacoes_io[DISCO].tempo_inicio = atoi(token);
                if (processo->operacoes_io[DISCO].tempo_inicio >= 0) {
                    processo->num_operacoes_io++;
                    processo->operacoes_io[DISCO].presente = 1;
                }
                break;
            case 4:
                // Tempo de início da operação de Fita
                processo->operacoes_io[FITA].tempo_inicio = atoi(token);
                if (processo->operacoes_io[FITA].tempo_inicio >= 0) {
                    processo->num_operacoes_io++;
                    processo->operacoes_io[FITA].presente = 1;
                }
                break;
            default:
                // Ignora tokens adicionais
                break;
        }
        idx++;
        token = strtok(NULL, ",");
    }

    if (idx < 3) {
        return 0; // Dados insuficientes
    }

    if (processo->num_operacoes_io > 0) {
        int primeira_io = processo->tempo_cpu + 1;
        for (int i = 0; i < QUANTIDADE_TIPOS_IO; ++i) {
            if (processo->operacoes_io[i].presente && processo->operacoes_io[i].tempo_inicio < primeira_io) {
                primeira_io = processo->operacoes_io[i].tempo_inicio;
                processo->tipo_io_atual = i;
            }
        }
    }

    return 1;
}
