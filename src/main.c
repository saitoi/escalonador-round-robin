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

    ListaFila lista_filas;
    ListaProcessos lista_processos;

    inicializa_lista_filas(&lista_filas);

    processa_menu(&lista_processos);

    escalonador(&lista_filas, &lista_processos);

    return 0;
}
