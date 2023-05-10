/*
 * Riccardo Rosin 284211
 * Lab 4 Es 1
*/
#include<stdio.h>
#include<stdlib.h>
#define FILE_IN "grafo.txt"

typedef enum bool{false, true} bool;

void initGrafo(int*** grafo, int* nNodi, int* nArchi, char* fileIn, int** mark){
    int i;
    FILE *fin;
    if ((fin = fopen(fileIn, "r+")) == NULL){
        printf("Errore nell'apertura del file: \"fileIn\"");
        exit(1);
    }
    
    // salvo numero di nodi
    fscanf(fin, " %d %d ", nNodi, nArchi);

    *mark = malloc(*nNodi * sizeof(int));

    // alloco la tabella di nodi: ogni riga è un arco
    *grafo = (int**) malloc(*nArchi * sizeof(int*));
    for(i=0; i<*nArchi; i++) (*grafo)[i] = (int*) malloc(2 * sizeof(int));

    // leggo ogni riga
    for(i=0; i<*nArchi; i++) fscanf(fin, " %d %d ", &(*grafo)[i][0], &(*grafo)[i][1]);

    fclose(fin);
}

void dealloca(int*** grafo, int n, int** mark){
    for(int i=0; i<n; i++) free((*grafo)[i]);
    free(*grafo);
    free(*mark);
}

bool verify(int mark[], int** grafo, int nArchi){
    int i;

    for(i=0; i<nArchi; i++){ // per ogni arco
        if(mark[grafo[i][0]] != 1 && mark[grafo[i][1]] != 1){
            return false;
        }
    }
    return true;
}

void calcola(int pos, int** grafo, int nNodi, int nArchi, int* mark){
    int i;
    if(pos==nNodi){
        // condizione terminazione
        if(verify(mark, grafo, nArchi)){
            for(i=0; i<nNodi; i++){
                if(mark[i] == 1) printf("%d ", i);
            }
            printf("\n");
        }
        return;
    }

    mark[pos] = 1;
    calcola(pos+1, grafo, nNodi, nArchi, mark);
    mark[pos] = 0;
    calcola(pos+1, grafo, nNodi, nArchi, mark);

}

int main(int argc, char* argv){
    int **grafo, nNodi, nArchi, *mark;
    initGrafo(&grafo, &nNodi, &nArchi, FILE_IN, &mark);

    calcola(0, grafo, nNodi, nArchi, mark);

    dealloca(&grafo, nArchi, &mark);
    return 0;
}