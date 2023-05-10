/*
 * Riccardo Rosin 284211
 * Lab 4 Es 4
*/
#include<stdio.h>
#include<stdlib.h>
#define FILE_IN /* "very_very_easy_test_set.txt" */ "test_set.txt"


/*
 * - uno zaffiro deve essere seguito immediatamente o da un altro zaffiro o da un rubino
 * - uno smeraldo deve essere seguito immediatamente o da un altro smeraldo o da un topazio
 * - un rubino deve essere seguito immediatamente o da uno smeraldo o da un topazio
 * - un topazio deve essere seguito immediatamente o da uno zaffiro o da un rubino
 *
 * - nessuna tipologia di pietra si può ripetere più di max_rip volte consecutive
 * - nella collana, il numero di zaffiri non può superare il numero di smeraldi
*/
enum gemme{zaffiro, rubino, topazio, smeraldo};
typedef enum {false, true} bool;


bool valida(int index, int this, int* collana, int maxRip){
    int count=1, // procedo come se avessi già messo la gemma attuale
        i, tmp = index-(maxRip + 1);
    for(i=index-1; i> tmp && i>=0; i--){
        if(collana[i] == this) count++;
        else break;
    }
    if(count > maxRip) return false;

    switch(collana[index-1]){
        case -1:
            return true;
        case zaffiro:
            return this==zaffiro || this==rubino;
        case smeraldo:
            return this==smeraldo || this==topazio;
        case rubino:
            return this==smeraldo || this==topazio;
        case topazio:
            return this==zaffiro || this==rubino;
    }
    return false;
}

bool finalTest(int* result, int dim){
    int zaf = 0, smer = 0, i;
    for(i=0; i<dim; i++){
        switch(result[i]){
            case zaffiro:
                zaf++;
                break;
            case smeraldo:
                smer++;
                break;
        }
    }
    return zaf <= smer;
}

void copyArray(int* arr1, int* arr2, int dim){
    for(int i=0; i<dim; i++) arr2[i] = arr1[i];
}

void stampaCollana(int* collana, int n){
    char str[] = "zrts";
    for(int i=0; i<n; i++) printf("%c", str[collana[i]]);
}

void funz(int pos, int val, int* maxVal, int* maxLen, int* pietre, int nPietre, int* values, int maxRip, int* collana, int* bestCollana){
    int i;


    for(i=0; i<4; i++){ // seleziona la pietra
        if(pos==0 || (valida(pos, i, collana, maxRip) && pietre[i]>0)){
            // la pietra soddisfa le condizioni

            pietre[i]--;                // segno che ho preso la pietra
            collana[pos] = i;           // la metto nella collana
            val+= values[i];         // ne valuto il valore

            if(val >= *maxVal && finalTest(collana, pos+1)){ // ho una nuova collana ottimale
                *maxVal = val;
                *maxLen = pos+1;
                copyArray(collana, bestCollana, pos+1);
            }

            // stampaCollana(collana, pos+1); printf("\n"); fflush(stdout);
            funz(pos+1, val, maxVal, maxLen, pietre, nPietre, values, maxRip, collana, bestCollana);

            pietre[i]++;
            val-= values[i];

        }
    }
}

void singleRun(FILE* fin, int test){
    int pietre[4], values[4], nPietre=0, maxRip, n=0, i=0, tot=0, *collana, *bestCollana;
    fscanf(fin, " %d %d %d %d %d %d %d %d %d",
        &pietre[zaffiro], &pietre[rubino], &pietre[topazio], &pietre[smeraldo],
        &values[zaffiro], &values[rubino], &values[topazio], &values[smeraldo],
        &maxRip
    );
    for(n=0; n < 4; n++) nPietre+= pietre[n]; // somma quante pietre ho in tutto
    if(nPietre == 0){printf("Test %d: Non è possibile generare una collana con 0 pietre disponibli", test); return;}

    collana = malloc(sizeof(int) * nPietre);
    bestCollana = malloc(sizeof(int) * nPietre);

    n=0; // n-> maxVal; i->maxLen
    funz(0, 0, &n, &i, pietre, nPietre, values, maxRip, collana, bestCollana);

    printf("Test %d: Soluzione ottima di valore %d usando %d gemma/e con occorrenze: ", test, n, i);
    stampaCollana(bestCollana, i);
    printf("\n");


    free(collana);
    free(bestCollana);
}

int main(int argc, char* argv){
    int i, n;

    FILE *fin;
    if ((fin = fopen(FILE_IN, "r+")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", FILE_IN);
        exit(-1);
    }
    fscanf(fin, " %d ", &n);

    for(int i = 0; i < n; i++) singleRun(fin, i+1);

    fclose(fin);

    return 0;
}
