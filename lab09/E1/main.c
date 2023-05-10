#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "item.h"
#include "grafo.h"


#define LENCMD (30+1)
#define FILE_IN "grafo4.txt"

typedef enum {false, true} Bool;


// Si scriva un programma C che svolga le seguenti operazioni:
//     - individuazione di tutti gli insiemi di archi di cardinalità minima la cui rimozione renda il grafo originale un DAG
//     - costruzione di un DAG rimuovendo, tra tutti gli insiemi di archi generati al passo precedente,
//       quelli dell’insieme a peso massimo. Il peso di un insieme di archi è definito come la somma dei
//       pesi degli archi in esso contenuti
//     - calcolo delle distanze massime da ogni nodo sorgente verso ogni nodo del DAG costruito al
//       passo precedente (cfr lucidi cap. 19).

Graph init(char* filename){
    Graph G;

    FILE *fp;
    if ((fp = fopen(filename, "r")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", filename);
        exit(-1);
    }
    
    G = GRAPH_load(fp);
    
    fclose(fp);
    return G;
}

int main(int argc, char* argv[]){

    Graph G = init(FILE_IN);

    if(GRAPH_isDAG(G)) return printf("Il grafo e' un DAG, nessuna operazione possibile.\n"), GRAPH_free(G), -1;


    EdgeIArrayWrapper result = GRAPH_individuazione(G);

    GRAPH_setDag(G, result);

    GRAPH_freeEdgeIArrayWrapper(result);

    GRAPH_free(G);

    return 1;
}