#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/processo.h"
#include "../include/utilitarios.h"

/* ************* INICIALIZAÇÃO ************* */

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

/* ************* CRIAÇÃO DE LISTA DE PROCESSOS ALEATÓRIOS ************* */

ListaProcessos criar_lista_processos_aleatorios(void) {
    ListaProcessos lista_processos = { .quantidade = MAXIMO_PROCESSOS };
    int i = 0;

    for (i = 0; i < lista_processos.quantidade; i++) {
        lista_processos.processos[i] = criar_processo_aleatorio();
        lista_processos.processos[i].pid = i;
    }

    return lista_processos;
}

Processo criar_processo_aleatorio(void) {
    int i;
    Processo processo;

    // Inicialização dos atributos do processo
    processo.instante_chegada = gerar_dado_aleatorio(0, TEMPO_MAX_CHEGADA);
    processo.tempo_cpu = gerar_dado_aleatorio(TEMPO_MIN_CPU, TEMPO_MAX_CPU);
    processo.tempo_cpu_restante = processo.tempo_cpu;
    processo.tempo_quantum_restante = 0;
    processo.tempo_cpu_atual = 0;
    processo.status_processo = PRONTO;
    processo.tipo_io_atual = -1; // Indica que não há operação de E/S atual
    processo.num_operacoes_io = 0; // Inicializa o contador de operações de E/S

    int tempo_io_primeiro = -1; // Para armazenar o tempo de início da primeira E/S

    // Inicializa as operações de E/S
    for (i = 0; i < QUANTIDADE_TIPOS_IO; ++i) {
        processo.operacoes_io[i].tipo_io = i; // 0 para DISCO, 1 para FITA
        processo.operacoes_io[i].presente = 0;
        processo.operacoes_io[i].tempo_inicio = -1;
        processo.operacoes_io[i].duracao_io = 0;
        processo.operacoes_io[i].tempo_restante = 0;

        /* Decide se o processo terá E/S ou não */
        int precisa_io = gerar_dado_aleatorio(0, 1);
        if (precisa_io) {   // E/S Presente
            processo.operacoes_io[i].duracao_io = seleciona_tempo_io(processo.operacoes_io[i].tipo_io);
            
            // Gera um tempo de início que não coincide com o primeiro tempo de E/S
            int tempo_inicio;
            do {
                tempo_inicio = gerar_dado_aleatorio(1, processo.tempo_cpu - 1);
            } while (tempo_inicio == tempo_io_primeiro);

            processo.operacoes_io[i].tempo_inicio = tempo_inicio;
            processo.operacoes_io[i].tempo_restante = processo.operacoes_io[i].duracao_io;
            processo.operacoes_io[i].presente = 1;
            processo.num_operacoes_io++;

            // Se for a primeira E/S, atualiza tempo_io_primeiro
            if (tempo_io_primeiro == -1) {
                tempo_io_primeiro = tempo_inicio;
            }
        }
    }

    /* Determina a operação de E/S atual: a com o menor tempo de início */
    int menor_tempo_inicio = processo.tempo_cpu + 1; // Inicializa com um valor maior que o máximo possível
    for (i = 0; i < QUANTIDADE_TIPOS_IO; ++i) {
        if (processo.operacoes_io[i].tempo_inicio >= 0 && processo.operacoes_io[i].tempo_inicio < menor_tempo_inicio) {
            menor_tempo_inicio = processo.operacoes_io[i].tempo_inicio;
            processo.tipo_io_atual = i; // i representa o tipo de E/S (0 = DISCO, 1 = FITA)
        }
    }

    return processo;
}
            
/* ************* CRIAÇÃO DE LISTA DE PROCESSOS CSV ************* */
            
ListaProcessos criar_lista_processos_csv(const char *nome_arquivo) {
    ListaProcessos lista_processos = { .quantidade = 0 };
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        enviar_mensagem_erro("Erro ao abrir arquivo de entrada.\n");
        return lista_processos;
    }

    char linha[MAX_LINHA];
    int quantidade_processos_lidos = 0;

    /* Ignora a primeira linha (cabeçalho) e checa por erros no arquivo */
    if (fgets(linha, MAX_LINHA, arquivo) == NULL) {
        fclose(arquivo);
        enviar_mensagem_erro("Erro ao ler o cabeçalho do arquivo.\n");
        return lista_processos;
    }

    while (fgets(linha, MAX_LINHA, arquivo) && quantidade_processos_lidos < MAXIMO_PROCESSOS) {
        if (!parse_linha_csv(linha, &lista_processos.processos[quantidade_processos_lidos])) {
            continue; /* Linha inválida, vai para a próxima */
        }
        quantidade_processos_lidos++;
    }

    fclose(arquivo);

    /* Nenhum processo foi identificado */
    if (quantidade_processos_lidos == 0) {
        enviar_mensagem_erro("Nenhum processo encontrado no CSV.\n");
        return lista_processos;
    }

    lista_processos.quantidade = quantidade_processos_lidos;

    return lista_processos;
}

int parse_linha_csv(char *linha, Processo *processo) {
    char *token;
    int idx = 0;
    int opcao_valida = 1;

    trim_novalinha(linha);

    if (strlen(linha) == 0)
        return 0;

    memset(processo, 0, sizeof(Processo));

    processo->tipo_io_atual = -1; /* Inicializa sem operação de E/S atual */ 
    processo->num_operacoes_io = 0;

    for (int i = 0; i < QUANTIDADE_TIPOS_IO; ++i) {
        processo->operacoes_io[i].tipo_io = i;
        processo->operacoes_io[i].presente = 0;
        processo->operacoes_io[i].duracao_io = seleciona_tempo_io(i);
        processo->operacoes_io[i].tempo_inicio = -1;
        processo->operacoes_io[i].tempo_restante = processo->operacoes_io[i].duracao_io;
    }

    token = strtok(linha, ",");
    while (token != NULL && idx < 5) {
        switch (idx) {
            case 0:
                processo->pid = converter_validar_int(token, 0, 10, &opcao_valida);
                break;
            case 1:
                processo->instante_chegada = converter_validar_int(token, 0, TEMPO_MAX_CHEGADA, &opcao_valida);
                break;
            case 2:
                processo->tempo_cpu = converter_validar_int(token, TEMPO_MIN_CPU, TEMPO_MAX_CPU, &opcao_valida);
                processo->tempo_cpu_restante = processo->tempo_cpu;
                processo->tempo_quantum_restante = 0;
                processo->tempo_cpu_atual = 0;
                processo->status_processo = PRONTO;
                break;
            case 3:
                // Tempo de início da operação de Disco
                processo->operacoes_io[DISCO].tempo_inicio = converter_validar_int(token, TEMPO_IO_PADRAO, processo->tempo_cpu - 1, &opcao_valida);
                if (processo->operacoes_io[DISCO].tempo_inicio >= 0) {
                    processo->num_operacoes_io++;
                    processo->operacoes_io[DISCO].presente = 1;
                }
                break;
            case 4:
                // Tempo de início da operação de Fita
                processo->operacoes_io[FITA].tempo_inicio = converter_validar_int(token, TEMPO_IO_PADRAO, processo->tempo_cpu - 1, &opcao_valida);
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

    if (!opcao_valida) {
        printf("%d", opcao_valida);
        enviar_mensagem_erro("Leitura de variáveis inválida (CSV)");
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

void trim_novalinha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && (str[len-1] == '\n' || str[len-1] == '\r')) {
        str[len-1] = '\0';
        if (len > 1 && str[len-2] == '\r') {
            str[len-2] = '\0';
        }
    }
}

/* ************* FUNÇÕES DO ESCALONADOR ************* */

void executa_processo(Processo *processo) {
    printf(" P%d executou por 1 u.t.\n", processo->pid);
    processo->tempo_cpu_restante -= 1;
    processo->tempo_cpu_atual += 1;
    processo->tempo_quantum_restante += 1;
    processo->status_processo = EXECUTANDO;
}

int processo_finalizado(Processo *processo) {
    if (processo->tempo_cpu_restante <= 0) {
        printf(" P%d finalizou sua execucao.\n", processo->pid);
        return 1;
    }
    return 0;
}

int tempo_inicio_io(Processo *processo) {
    if (processo->operacoes_io[processo->tipo_io_atual].presente == 1 && processo->num_operacoes_io != 0) {
        int tipo_io = processo->tipo_io_atual;

        int instante_inicio_io = processo->operacoes_io[tipo_io].tempo_inicio;

        if (instante_inicio_io == processo->tempo_cpu_atual)
            return 1;
        else
            return 0;
    } else {
        return 0;
    }
}

void executa_io(Processo *processo) {
    /* Se processo acaba de entrar em executando, Status passa a ser IO */
    if (processo->status_processo == EXECUTANDO)
        processo->status_processo = ENTRADA_SAIDA;
    else {
        processo->operacoes_io[processo->tipo_io_atual].tempo_restante -= 1;
        printf(" P%d executou 1 u.t. da sua E/S de %s, faltam %d u.t.\n", processo->pid,
               seleciona_tipo_io(processo->operacoes_io[processo->tipo_io_atual].tipo_io),
               processo->operacoes_io[processo->tipo_io_atual].tempo_restante);
    }
}

int io_finalizada(Processo *processo) {
    if (processo->operacoes_io[processo->tipo_io_atual].tempo_restante <= 0) {
        printf(" P%d finalizou sua E/S de %s,", processo->pid,
               seleciona_tipo_io(processo->tipo_io_atual));
        processo->tempo_quantum_restante = 0;
        processo->tipo_io_atual = (processo->num_operacoes_io == 2) ? 
            (processo->tipo_io_atual == DISCO ? FITA : DISCO) : TEMPO_IO_PADRAO;

        return 1;
    }
    return 0;
}

int tempo_quantum_completo(Processo *processo) {
    if (processo->tempo_quantum_restante == QUANTUM) {
        processo->tempo_quantum_restante = 0;
        return 1;
    }
    return 0;
}
