#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../include/utilitarios.h"
#include "../include/processo.h"

void enviar_mensagem_erro(const char *mensagem) {
    fprintf(stderr, "%s\n", mensagem);
    exit(1);
}

void obter_entrada_caractere(const char *mensagem, char *variavel, char min, char max) {
    char entrada;
    int valido = 0;

    while (!valido) {
        printf("%s (%c - %c): ", mensagem, min, max);
        
        entrada = getchar(); // Lê o caractere
        while (getchar() != '\n'); // Limpa o buffer

        // Verifica se o caractere está no intervalo permitido
        if (entrada >= min && entrada <= max) {
            *variavel = entrada;
            valido = 1;
        } else {
            printf("Caractere inválido. Tente novamente.\n");
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
        
        if (tolower(ch) == 'n') {
            printf("Encerrando o programa..");
            exit(0);
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
