#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Corrigir dependências malucas
#include "../include/fila.h"
#include "../include/processo.h"
#include "../include/escalonador.h"
#include "../include/interface.h"
#include "../include/auxiliar.h"



void processa_fila_prioridade(Fila *fila_prioridade, ListaFila *lista_filas, ListaProcessos *lista_processos, int instante_atual, int *processos_finalizados) {
    Processo *processo_atual = desenfileira(fila_prioridade);
    if (processo_atual == NULL) return; // Verifica se a fila estava vazia

    executa_processo(processo_atual);

    if (processo_finalizado(processo_atual)) {
        atualiza_turnaround(processo_atual, instante_atual);
        (*processos_finalizados)++;
    } 
    else if (tempo_inicio_io(processo_atual)) {
        envia_para_io(processo_atual, lista_filas);
    } 
    else if (tempo_quantum_completo(processo_atual)) {
        printf(" P%d sofreu preempcao, vai pra fila de baixa prioridade.\n", processo_atual->pid);
        enfileira(&lista_filas->filas[FILA_BAIXA_PRIORIDADE], processo_atual);
    } 
    else {
        enfileira_inicio(fila_prioridade, processo_atual);
    }
}


void processa_fila_io(Fila *fila_io, Fila *fila_destino, const char *prioridade, int instante_atual, int *processos_finalizados) {
    if (!fila_vazia(fila_io)) {
        Processo *processo_io = desenfileira(fila_io);
        if (processo_io == NULL) return; // Verifica se a fila estava vazia

        executa_io(processo_io);

        if (io_finalizada(processo_io)) {
            printf(" P%d completou I/O e vai para a fila de %s prioridade.\n", processo_io->pid, prioridade);
            enfileira(fila_destino, processo_io);
        } 
        else if (processo_finalizado(processo_io)) {
            atualiza_turnaround(processo_io, instante_atual);
            (*processos_finalizados)++;
        }
        else {
            enfileira_inicio(fila_io, processo_io);
        }
    }
}


void escalonador(ListaFila *lista_filas, ListaProcessos *lista_processos) {
    int instante_atual = 0;
    int processos_finalizados = 0;

    imprime_informacao_processos(lista_processos);
    confirmacao_usuario();

    while (processos_finalizados < lista_processos->quantidade) {
        imprime_instante(instante_atual);
        verifica_novos_processos(&lista_filas->filas[FILA_ALTA_PRIORIDADE], lista_processos, instante_atual);

        /* Coordena filas de baixa/alta prioridades */
        if (!fila_vazia(&lista_filas->filas[FILA_ALTA_PRIORIDADE]))
            processa_fila_prioridade(&lista_filas->filas[FILA_ALTA_PRIORIDADE], lista_filas, lista_processos, instante_atual, &processos_finalizados);
        else if (!fila_vazia(&lista_filas->filas[FILA_BAIXA_PRIORIDADE]))
            processa_fila_prioridade(&lista_filas->filas[FILA_BAIXA_PRIORIDADE], lista_filas, lista_processos, instante_atual, &processos_finalizados);

        /* Coordena filas de disco/fita */
        processa_fila_io(&lista_filas->filas[FILA_DISCO], &lista_filas->filas[FILA_BAIXA_PRIORIDADE], "baixa", instante_atual, &processos_finalizados);
        processa_fila_io(&lista_filas->filas[FILA_FITA], &lista_filas->filas[FILA_ALTA_PRIORIDADE], "alta", instante_atual, &processos_finalizados);

        if (todas_filas_vazias(*lista_filas))
            printf("Nenhuma fila com processos, o processador está ocioso.\n");
        else
            imprime_todas_filas(lista_filas);

        instante_atual++;
        if (instante_atual == 30)
            break;
    }

    imprime_fim_escalonador();
    imprime_turnaround_processos(*lista_processos);
}


// void escalonador(ListaProcessos *lista, Fila *fila_alta_prioridade, Fila *fila_baixa_prioridade, Fila *fila_disco, Fila *fila_fita) {
//     int tempo_atual = 0, num_processos_finalizados = 0;

//     // Exibe informações iniciais dos processos
//     imprime_informacao_processos(*lista);
//     confirmacao_usuario();

//     while (num_processos_finalizados < lista->quantidade) {
//         imprime_instante(tempo_atual);
//         verifica_novos_processos(lista->processos, tempo_atual, fila_alta_prioridade);

//         // Processa filas de prioridade
//         processa_fila_prioridade(fila_alta_prioridade, fila_alta_prioridade, fila_baixa_prioridade, fila_disco, fila_fita, &num_processos_finalizados, tempo_atual, lista->processos);
//         processa_fila_prioridade(fila_baixa_prioridade, fila_alta_prioridade, fila_baixa_prioridade, fila_disco, fila_fita, &num_processos_finalizados, tempo_atual, lista->processos);

//         // Processa filas de I/O
//         processa_fila_io(fila_disco, fila_baixa_prioridade, "baixa");
//         processa_fila_io(fila_fita, fila_alta_prioridade, "alta");

//         // Verifica se todas as filas estão vazias
//         if (!verifica_processador(fila_alta_prioridade, fila_baixa_prioridade) && !verifica_io(fila_disco, fila_fita)) {
//             printf("Nenhuma fila com processos, o processador está ocioso.\n");
//         } else {
//             imprime_todas_filas(fila_alta_prioridade, fila_baixa_prioridade, fila_disco, fila_fita);
//         }

//         tempo_atual++;
//     }

//     printf("\nNão há mais processos; o escalonamento foi finalizado com sucesso.\n");
//     printf("\n============ FIM DO ESCALONAMENTO ============\n");
//     printf("\nTempos de turnaround de cada um dos processos:\n\n");
//     imprime_turnaround_processos(lista->processos, lista->quantidade);

//     free(lista->processos);
// }

// void escalonador(ListaProcessos *lista, Fila *fila_alta_prioridade, Fila *fila_baixa_prioridade, Fila *fila_disco, Fila *fila_fita) {
//     int tempo_atual = 0,
//         num_processos_finalizados = 0;

//     Processo processo_atual,
//              entrada_saida_atual;

//     // Exibe informações iniciais dos processos e pede confirmação do usuário
//     imprime_informacao_processos(*lista);
//     confirmacao_usuario();

//     while (num_processos_finalizados < lista->quantidade) {
//         imprime_instante(tempo_atual);
//         verifica_novos_processos(lista->processos, tempo_atual, fila_alta_prioridade);

//         if (!fila_vazia(fila_alta_prioridade)) {
//             processo_atual = desenfileira(fila_alta_prioridade);
//             executa_processo(&processo_atual);

//             /* Verifica se algum processo finalizou */
//             if (processo_finalizado(&processo_atual)) {
//                 atualiza_turnaround(&processo_atual, tempo_atual);
//                 num_processos_finalizados++;
//             }

//             /* Verifica inicio de operacao de I/O */
//             else if (tempo_inicio_io(&processo_atual))
//                 envia_para_io(processo_atual, fila_disco, fila_fita);

//             /* Verifica finalização do quantum, c.c. vai para fila de baixa prioridade */
//             else if (tempo_quantum_completo(&processo_atual, QUANTUM)) {
//                 printf(" P%d sofreu preempcao, vai pra fila de baixa prioridade.\n",
//                     processo_atual.pid);
//                 enfileira(fila_baixa_prioridade, processo_atual);
//             }

//             else
//                 enfileira_inicio(fila_alta_prioridade, processo_atual);
//         }

//         else if (!fila_vazia(fila_baixa_prioridade)) {
//             processo_atual = desenfileira(fila_baixa_prioridade);
//             executa_processo(&processo_atual);

//             if (processo_finalizado(&processo_atual)) {
//                 atualiza_turnaround(&processo_atual, tempo_atual);
//                 num_processos_finalizados++;
//             }

//             else if (tempo_inicio_io(&processo_atual))
//                 envia_para_io(processo_atual, fila_disco, fila_fita);

//             else if (tempo_quantum_completo(&processo_atual, QUANTUM)) {
//                 printf(" P%d sofreu preempcao, vai pra fila de baixa prioridade.\n",
//                     processo_atual.pid);
//                 enfileira(fila_baixa_prioridade, processo_atual);
//             }

//             else
//                 enfileira_inicio(fila_baixa_prioridade, processo_atual);
//         }

//         if (!fila_vazia(fila_disco)) {
//             entrada_saida_atual = desenfileira(fila_disco);
//             executa_io(&entrada_saida_atual);

//             if (io_finalizada(&entrada_saida_atual)) {
//                 printf(" vai para a fila de baixa prioridade.\n");
//                 enfileira(fila_baixa_prioridade, entrada_saida_atual);
//             } else
//                 enfileira_inicio(fila_disco, entrada_saida_atual);
//         }

//         if (!fila_vazia(fila_fita)) {
//             entrada_saida_atual = desenfileira(fila_fita);
//             executa_io(&entrada_saida_atual);

//             if (io_finalizada(&entrada_saida_atual)) {
//                 printf(" vai para a fila de alta prioridade.\n"); // Essa parte não fez tanto sentido..
//                 if (!fila_vazia(fila_baixa_prioridade) && processo_atual.pid == fila_baixa_prioridade->inicio->processo.pid) printf(" P%d sofreu preempcao, vai pra fila de baixa prioridade.\n",
//                     processo_atual.pid);
//                 enfileira(fila_alta_prioridade, entrada_saida_atual);
//             } else
//                 enfileira_inicio(fila_fita, entrada_saida_atual);
//         }

//         if (!verifica_processador(fila_alta_prioridade, fila_baixa_prioridade) &&
//             !verifica_io(fila_disco, fila_fita)) {
//             printf("Nenhuma fila com processos, o processador esta ocioso.\n");
//         } else
//             imprime_todas_filas(fila_alta_prioridade, fila_baixa_prioridade, fila_disco, fila_fita);

//         tempo_atual++;
//     }

//     imprime_fim_escalonador();
//     imprime_turnaround_processos(lista->processos, lista->quantidade);

//     free(lista->processos);
// }

void imprime_informacao_processos(ListaProcessos *lista_processos) {
    if (lista_processos->quantidade > 0) {
        printf("\n═══════════════════════════════════════ PROCESSOS ═══════════════════════════════════════\n");
        printf(" PID\tTempo de Início\t\tTempo de Serviço\tE/S (Tempo de Inicio)\t\t\n");
        printf("─────────────────────────────────────────────────────────────────────────────────────────\n");

        for (int i = 0; i < lista_processos->quantidade; i++) {
            printf(" P%d\t\t%d\t\t\t%d\t\t%d", 
                lista_processos->processos[i].pid, 
                lista_processos->processos[i].instante_chegada,
                lista_processos->processos[i].tempo_cpu,
                lista_processos->processos[i].tipo_io_atual);

            if (lista_processos->processos[i].num_operacoes_io == 0) {
                printf("Nenhuma operacao de E/S.");
            } else {
                for (int j = 0; j < QUANTIDADE_TIPOS_IO; j++) {
                    int tempo_inicio_io = lista_processos->processos[i].operacoes_io[j].tempo_inicio;
                    if (tempo_inicio_io == -1)
                        continue;
                    printf("%s (%d)", seleciona_tipo_io(lista_processos->processos[i].operacoes_io[j].tipo_io), tempo_inicio_io);

                    if (j < lista_processos->processos[i].num_operacoes_io - 1) {
                        printf(", ");
                    }
                }
            }
            printf("\n");
        }

        printf("═════════════════════════════════════════════════════════════════════════════════════════\n");
    } else {
        printf("Sem processos.\n");
    }
}


void confirmacao_usuario(void) {
    char confirmacao;
    valida_entrada_char("Aqui estão os dados dos processos, deseja prosseguir? ", &confirmacao);
    if (toupper(confirmacao) != 'N')
        return;
    else
        exit(1);
}

void verifica_novos_processos(Fila *fila_alta_prioridade, ListaProcessos *lista_processos, int instante_atual) {
    for (int i = 0; i < lista_processos->quantidade; ++i) {
        if (lista_processos->processos[i].instante_chegada == instante_atual) {
            enfileira(fila_alta_prioridade, &lista_processos->processos[i]);
            printf(" P%d entrou na fila de alta prioridade.\n", lista_processos->processos[i].pid);
        }
    }
}


// void verifica_novos_processos(Processo *processos, int tempo_atual, Fila *fila_alta_prioridade) {
//     int i = 0;
//     for (i = 0; i < MAXIMO_PROCESSOS; i++)
//         if (processos[i].instante_chegada == tempo_atual) {
//             enfileira(fila_alta_prioridade, processos[i]);
//             printf(" P%d entrou na fila de alta prioridade.\n", processos[i].pid);
//         }
// }

void imprime_turnaround_processos(ListaProcessos lista_processos) {
    float turnaround_medio = 0,
          tempo_espera_medio = 0;

    printf("\n═══════════════════════════════════════ TURNAROUND ══════════════════════════════════════\n");
    printf(" PID\tTempo de Turnaround\tTempo de Espera\n");
    printf("─────────────────────────────────────────────────────────────────────────────────────────\n");

    for (int i = 0; i < lista_processos.quantidade; i++) {
        printf(" P%d\t\t%d u.t.\t\t %d u.t.\n", 
            lista_processos.processos[i].pid, 
            lista_processos.processos[i].tempo_turnaround, 
            lista_processos.processos[i].tempo_turnaround - lista_processos.processos[i].tempo_cpu);
        turnaround_medio += (float) lista_processos.processos[i].tempo_turnaround;
        tempo_espera_medio += (float) lista_processos.processos[i].tempo_turnaround - lista_processos.processos[i].tempo_cpu;
    }

    printf("═════════════════════════════════════════════════════════════════════════════════════════\n");
    printf(" Turnaround médio: %.2f u.t.\n", turnaround_medio / lista_processos.quantidade);
    printf(" Tempo de espera médio: %.2f u.t.\n", tempo_espera_medio / lista_processos.quantidade);
}

void imprime_tempos_turnaround(Processo *processos) {
    float turnaround_medio = 0;
    int i = 0;
    for (i = 0; i < MAXIMO_PROCESSOS; i++) {
        printf("P%d: %d u.t\n", processos[i].pid, processos[i].tempo_turnaround);
        turnaround_medio += (float) processos[i].tempo_turnaround;
    }
    printf("\nTurnaround medio: %.2f u.t.\n", turnaround_medio / MAXIMO_PROCESSOS);
}

int todas_filas_vazias(ListaFila lista_filas) {
    int i;
    for (i = 0; i < NUM_FILAS; ++i) {
        if (!fila_vazia(&lista_filas.filas[i]))
            return 0;
    }
    return 1;
}

void envia_para_io(Processo *processo, ListaFila *lista_filas) {
    printf("%d", processo->tipo_io_atual);
    printf(" P%d foi para a fila de E/S (%s).\n", processo->pid, seleciona_tipo_io(processo->tipo_io_atual));

    switch (processo->tipo_io_atual) {
        case DISCO:
            if (!fila_vazia(&lista_filas->filas[FILA_DISCO]))
                processo->status_processo = ENTRADA_SAIDA;
            enfileira(&lista_filas->filas[FILA_DISCO], processo);
            break;
        case FITA:
            if (!fila_vazia(&lista_filas->filas[FILA_FITA]))
                processo->status_processo = ENTRADA_SAIDA;
            enfileira(&lista_filas->filas[FILA_FITA], processo);
            break;
    }
}

void atualiza_turnaround(Processo *processo_atual, int instante_fim) {
    processo_atual->tempo_turnaround = instante_fim - processo_atual->instante_chegada;
    printf("[ TURNAROUND DE P%d ] : %d\n", processo_atual->pid, processo_atual->tempo_turnaround);
}
