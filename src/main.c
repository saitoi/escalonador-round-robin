#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/processo.h"
#include "../include/fila.h"
#include "../include/escalonador.h"
#include "../include/interface.h"
#include "../include/auxiliar.h"

int main(void) {
    Processo *processos = inicializa_processos(MAXIMO_PROCESSOS);

    if (processos == NULL) {
        tratar_erro_alocacao("Erro ao alocar processos iniciais.\n");
        exit(1);
    }

    Fila fila_alta_prioridade, fila_baixa_prioridade, fila_disco, fila_fita, fila_impressora;

    srand(time(NULL));

    inicializa_fila(&fila_alta_prioridade);
    inicializa_fila(&fila_baixa_prioridade);
    inicializa_fila(&fila_disco);
    inicializa_fila(&fila_fita);
    inicializa_fila(&fila_impressora);

    processa_menu(processos,
                    &fila_alta_prioridade,
                    &fila_baixa_prioridade,
                    &fila_disco, 
                    &fila_fita, 
                    &fila_impressora);

    esvazia_fila(&fila_alta_prioridade);
    esvazia_fila(&fila_baixa_prioridade);
    esvazia_fila(&fila_disco);
    esvazia_fila(&fila_fita);
    esvazia_fila(&fila_impressora);

    return 0;
}
