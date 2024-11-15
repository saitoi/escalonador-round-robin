#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../include/utilitarios.h"
#include "../include/processo.h"

void enviar_mensagem_erro(const char *mensagem) {
    fprintf(stderr, "%s\n", mensagem);
    exit(1);
}

void valida_entrada_inteiro(int valor, int min, int max, int *opcao_valida) {
    if (valor < min || valor > max) {
        printf("Entrada inválida, escolha um valor dentro do intervalo (%d - %d).\n", min, max);
        *opcao_valida = 0;
    } else {
        *opcao_valida = 1;
    }
}

void obter_entrada_inteiro(const char *mensagem, int *variavel, int min, int max) {
    int opcao_valida = 0;
    int valor;
    char ch;

    while (1) { 
        printf("%s (%d - %d): ", mensagem, min, max);
        if (scanf("%d", &valor) != 1) {
            printf("Caractere inválido. Tente novamente..\n");
            while ((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }


        valida_entrada_inteiro(valor, min, max, &opcao_valida);

        if (opcao_valida) {
            *variavel = valor;
            break;
        }
        else {
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
     }
}

int converter_validar_int(const char *str, int min, int max, int *opcao_valida) {
    char *endptr;
    long tmp = strtol(str, &endptr, 10);

    if (*str != '\0' && *endptr == '\0' && tmp >= min && tmp <= max) {
        return (int)tmp;
    } else {
        *opcao_valida = 0;
        return -1;
    }
}


void valida_entrada_char(void) {
    char ch;
    int opcao_valida = 0;

    printf("Aqui estão os dados dos processos, deseja prosseguir? (s/N): ");
    while (!opcao_valida) { 
        if (scanf(" %c", &ch) != 1 || (tolower(ch) != 's' && tolower(ch) != 'n')) {
            printf("Entrada inválida. Tente novamente.. (s/N):");
            while ((ch = getchar()) != '\n' && ch != EOF);  // Limpa buffer de entrada
            continue;
        }
        
        opcao_valida = 1;
    }
}

int gerar_dado_aleatorio(int min, int max) {
    return min + rand() % (max - min + 1);
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
