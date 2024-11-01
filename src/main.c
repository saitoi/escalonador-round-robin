#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// TODO: Distribuir melhor as funções, evitar chamar todos os headers
#include "../include/processo.h"
#include "../include/fila.h"
#include "../include/escalonador.h"
#include "../include/interface.h"
#include "../include/auxiliar.h"

int main(void) {

    srand(time(NULL));

    Fila fila_alta_prioridade, fila_baixa_prioridade, fila_disco, fila_fita;

    inicializa_fila(&fila_alta_prioridade);
    inicializa_fila(&fila_baixa_prioridade);
    inicializa_fila(&fila_disco);
    inicializa_fila(&fila_fita);

    processa_menu(&fila_alta_prioridade,
                    &fila_baixa_prioridade,
                    &fila_disco, 
                    &fila_fita);

    esvazia_fila(&fila_alta_prioridade);
    esvazia_fila(&fila_baixa_prioridade);
    esvazia_fila(&fila_disco);
    esvazia_fila(&fila_fita);

    return 0;
}
