#include <stdio.h>
#include <stdlib.h>

#include "../include/escalonador.h"
#include "../include/processo.h"
#include "../include/fila.h"

#define QUANTUM 2 /* Quantum de tempo */
#define MAXIMO_PROCESSOS 4 /* Numero maximo de processos */

void escalonador(Processo *processos, Fila *fila_alta_prioridade, Fila *fila_baixa_prioridade, Fila *fila_disco, Fila *fila_fita, Fila *fila_impressora) {

    int tempo_atual = 0,
        num_processos_finalizados = 0;

    Processo processo_atual,
             entrada_saida_atual;

    imprime_informacao_processos(processos);

    while (num_processos_finalizados < MAXIMO_PROCESSOS) {
        printf("\n=========== INSTANTE %d ===========\n\n", tempo_atual);

        verifica_novos_processos(processos, tempo_atual, fila_alta_prioridade);

        if (!fila_vazia(fila_alta_prioridade)) {
            processo_atual = desenfileira(fila_alta_prioridade);
            executa_processo(&processo_atual);

            if (processo_finalizado(&processo_atual)) {
                atualiza_tempo_turnaround(&processo_atual, tempo_atual, processos);
                num_processos_finalizados++;
            }

            else if (tempo_inicio_es(&processo_atual))
                envia_para_es(processo_atual, fila_disco, fila_fita, fila_impressora);

            else if (tempo_quantum_completo(&processo_atual, QUANTUM)) {
                printf("O processo P%d sofreu preempcao, vai pra fila de baixa prioridade.\n",
                    processo_atual.pid);
                enfileira(fila_baixa_prioridade, processo_atual);
            }

            else
                enfileira_inicio(fila_alta_prioridade, processo_atual);
        }

        else if (!fila_vazia(fila_baixa_prioridade)) {
            processo_atual = desenfileira(fila_baixa_prioridade);
            executa_processo(&processo_atual);

            if (processo_finalizado(&processo_atual)) {
                atualiza_tempo_turnaround(&processo_atual, tempo_atual, processos);
                num_processos_finalizados++;
            }

            else if (tempo_inicio_es(&processo_atual))
                envia_para_es(processo_atual, fila_disco, fila_fita, fila_impressora);

            else if (tempo_quantum_completo(&processo_atual, QUANTUM)) {
                printf("O processo P%d sofreu preempcao, vai pra fila de baixa prioridade.\n",
                    processo_atual.pid);
                enfileira(fila_baixa_prioridade, processo_atual);
            }

            else
                enfileira_inicio(fila_baixa_prioridade, processo_atual);
        }

        if (!fila_vazia(fila_disco)) {
            entrada_saida_atual = desenfileira(fila_disco);
            executa_es(&entrada_saida_atual);

            if (es_finalizada(&entrada_saida_atual)) {
                printf(" vai para a fila de baixa prioridade.\n");
                enfileira(fila_baixa_prioridade, entrada_saida_atual);
            } else
                enfileira_inicio(fila_disco, entrada_saida_atual);
        }

        if (!fila_vazia(fila_fita)) {
            entrada_saida_atual = desenfileira(fila_fita);
            executa_es(&entrada_saida_atual);

            if (es_finalizada(&entrada_saida_atual)) {
                printf(" vai para a fila de alta prioridade.\n");
                if (!fila_vazia(fila_baixa_prioridade) && processo_atual.pid == fila_baixa_prioridade->inicio->processo.pid) printf("O processo P%d sofreu preempcao, vai pra fila de baixa prioridade.\n",
                    processo_atual.pid);
                enfileira(fila_alta_prioridade, entrada_saida_atual);
            } else
                enfileira_inicio(fila_fita, entrada_saida_atual);
        }

        if (!fila_vazia(fila_impressora)) {
            entrada_saida_atual = desenfileira(fila_impressora);
            executa_es(&entrada_saida_atual);

            if (es_finalizada(&entrada_saida_atual)) {
                printf(" vai para a fila de alta prioridade.\n");
                if (!fila_vazia(fila_baixa_prioridade) && processo_atual.pid == fila_baixa_prioridade->inicio->processo.pid) printf("O processo P%d sofreu preempcao, vai pra fila de baixa prioridade.\n",
                    processo_atual.pid);
                enfileira(fila_alta_prioridade, entrada_saida_atual);
            } else
                enfileira_inicio(fila_impressora, entrada_saida_atual);
        }

        if (!verifica_processador(fila_alta_prioridade, fila_baixa_prioridade) &&
            !verifica_es(fila_disco, fila_fita, fila_impressora)) {
            printf("Nenhuma fila com processos, o processador esta ocioso.\n");
        } else
            imprime_filas(fila_alta_prioridade, fila_baixa_prioridade, fila_disco, fila_fita, fila_impressora);

        tempo_atual++;
    }

    printf("\nNao ha mais processos, e portanto, o escalonamento foi finalizado com sucesso.\n");

    printf("\n============ FIM DO ESCALONAMENTO ============\n");
    printf("\nTempos de turnaround de cada um dos processos: \n\n");
    imprime_tempos_turnaround(processos);

    free(processos);
}

void imprime_informacao_processos(Processo *processos) {
    if (processos != NULL) {
        int i = 0;

        printf("================================== PROCESSOS ==================================\n");

        printf("\nPID\tTempo de servico\tTempo de inicio\t\tE/S (Tempo de inicio)\n");

        for (i = 0; i < MAXIMO_PROCESSOS; i++) {
            printf("P%d\t\t%d\t\t\t%d\t\t", processos[i].pid, processos[i].tempo_cpu, processos[i].tempo_inicio);

            if (processos[i].num_operacoes_es == 0)
                printf("Nenhuma operacao de E/S.");
            else {
                int j = 0;
                for (j = 0; j < processos[i].num_operacoes_es; j++) {
                    printf("%s (%d)", seleciona_tipo_es(processos[i].operacoes_es[j].tipo_es), processos[i].operacoes_es[j].tempo_inicio);

                    if (j < processos[i].num_operacoes_es - 1)
                        printf(", ");
                }
            }
            printf("\n");
        }
    } else printf("Sem processos.\n");
}

void imprime_informacao_processos(Processo *processos) {
    if (processos != NULL) {
        
    } else printf("Sem processos.\n");
}


void verifica_novos_processos(Processo *processos, int tempo_atual, Fila *fila) {
    int i = 0;
    for (i = 0; i < MAXIMO_PROCESSOS; i++)
        if (processos[i].tempo_inicio == tempo_atual) {
            enfileira(fila, processos[i]);
            printf("O processo P%d entrou na fila de alta prioridade.\n", processos[i].pid);
        }
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

int verifica_processador(Fila *fila_alta_prioridade, Fila *fila_baixa_prioridade) {
    return !fila_vazia(fila_alta_prioridade) || !fila_vazia(fila_baixa_prioridade);
}

int verifica_es(Fila *fila_disco, Fila *fila_fita, Fila *fila_impressora) {
    return !fila_vazia(fila_disco) || !fila_vazia(fila_fita) || !fila_vazia(fila_impressora);
}

void envia_para_es(Processo processo, Fila *fila_disco, Fila *fila_fita, Fila *fila_impressora) {
    printf("O processo P%d foi para a fila de E/S (%s).\n", processo.pid, seleciona_tipo_es(processo.operacoes_es[processo.operacao_es_atual].tipo_es));

    switch (processo.operacoes_es[processo.operacao_es_atual].tipo_es) {
        case DISCO:
            if (!fila_vazia(fila_disco))
                processo.status_processo = ENTRADA_SAIDA;
            enfileira(fila_disco, processo);
            break;

        case FITA:
            if (!fila_vazia(fila_fita))
                processo.status_processo = ENTRADA_SAIDA;
            enfileira(fila_fita, processo);
            break;

        case IMPRESSORA:
            if (!fila_vazia(fila_impressora))
                processo.status_processo = ENTRADA_SAIDA;
            enfileira(fila_impressora, processo);
            break;
    }
}

void atualiza_tempo_turnaround(Processo *processo, int turnaround, Processo *processos) {
    int i = 0;
    for (i = 0; i < MAXIMO_PROCESSOS; i++)
        if (processos[i].pid == processo->pid)
            processos[i].tempo_turnaround = turnaround - processos[i].tempo_inicio;
}
