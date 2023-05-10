#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "grafo.h"
#include "ST.h"
#include "item.h"

#define ROW 200
#define LENCMD (30+1)

typedef enum {stampa, sottografo, conversione, fine} Comando;
typedef enum {false, true} Bool;


Graph init(char* filename){
    int V=0;
    char* row, *tmp;
    row = malloc(ROW*sizeof(char));
    tmp = malloc(ROW*sizeof(char));

    FILE *fp;
    if ((fp = fopen(filename, "r")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", filename);
        exit(-1);
    }
    
    while(!feof(fp)){
        fgets(row, ROW, fp);
        if(sscanf(row, "%s", tmp) == 1)
            V++;
    }
    fclose(fp);
    // V = 21;
    Graph G = GRAPH_init(V);
    GRAPH_initM(G);

    if ((fp = fopen(filename, "r")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", filename);
        exit(-1);
    }
    
    GRAPH_readM(G, fp);

    fclose(fp);
    free(row);
    free(tmp);

    return G;
}

Comando leggiComando(){
    char cmd[LENCMD];
    scanf("%s", cmd);
    if(strcmp(cmd, "stampa") == 0) return stampa;
    if(strcmp(cmd, "sottografo") == 0) return sottografo;
    if(strcmp(cmd, "conversione") == 0) return conversione;
    if(strcmp(cmd, "fine") == 0 || strcmp(cmd, "end") == 0 || strcmp(cmd, "stop") == 0 || strcmp(cmd, "cls") == 0 || strcmp(cmd, "exit") == 0) return fine;
    return -1;
}

int main(int argc, char* argv[]){
    // if(argc != 2){
    //     printf("Usage: executable <filename>\n", argv[0]);
    //     return -1;
    // }

    // Graph G = init(argv[1]);
    Graph G = init("grafo.txt");
    Bool Terminazione = false;
    Item A, B, C;

    printf("\n\n");
    G_stampaMatrice(G);
    printf("\n\n");

    printf("Inizializzazione completata. Comandi disponibili: stampa, sottografo, conversione, fine.\n");
    while(!Terminazione){
        printf(">>> ");
        switch(leggiComando()){
            case stampa:
                GRAPH_show(G);
                break;
            case sottografo:

                printf("Inserisci la chiave del primo vertice: "); A = ITEM_scanKey(stdin);
                printf("Inserisci la chiave del secondo vertice: "); B = ITEM_scanKey(stdin);
                printf("Inserisci la chiave del terzo vertice: "); C = ITEM_scanKey(stdin);

                if(GRAPH_isSubgraph(G, A, B, C))
                    printf("I vertici inseriti formano un sottografo.\n");
                else
                    printf("I vertici inseriti non formano un sottografo.\n");

                break;
            case conversione:
                GRAPH_convert(G);
                printf("Conversione eseguita.\n");
                break;
            case fine:
                Terminazione = true;
                break;
            default:
                printf("Comando non riconosciuto.\n");
                break;
        }
    }

    GRAPH_free(G);

    return 0;
}

