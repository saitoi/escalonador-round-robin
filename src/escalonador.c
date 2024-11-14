#include <stdio.h>
#include <stdlib.h>

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
    valida_entrada_char();

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
    }

    imprime_fim_escalonador();
    imprime_turnaround_processos(*lista_processos);
}


void verifica_novos_processos(Fila *fila_alta_prioridade, ListaProcessos *lista_processos, int instante_atual) {
    for (int i = 0; i < lista_processos->quantidade; ++i) {
        if (lista_processos->processos[i].instante_chegada == instante_atual) {
            enfileira(fila_alta_prioridade, &lista_processos->processos[i]);
            printf(" P%d entrou na fila de alta prioridade.\n", lista_processos->processos[i].pid);
        }
    }
}

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
    // printf("%d", processo->tipo_io_atual);
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
    processo_atual->tempo_turnaround = instante_fim - processo_atual->instante_chegada + 1;
    printf("\n[ TURNAROUND DE P%d ] : %d\n\n", processo_atual->pid, processo_atual->tempo_turnaround);
}
