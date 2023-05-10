/*
 * Riccardo Rosin 284211
 * Lab 4 Es 3 (con truccetto)
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define FILE_IN /* "very_very_easy_test_set.txt" *//* "very_easy_test_set.txt" *//* "easy_test_set.txt" */"hard_test_set.txt"

enum gemme{zaffiro, rubino, topazio, smeraldo};
typedef enum {false, true} bool;

bool valida(int prev, int this){
    switch(prev){
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

int min(int* a){
    int min, i;
    for(i=1, min = a[0]; i<4; i++)
        if(a[i] < min) min = a[i];
    return min;
}

void stampaCollana(int* collana, int n){
    char str[] = "zrts";
    for(int i=0; i<n; i++) printf("%c", str[collana[i]]);
}

void funz(int pos, int* max, int n, int pietre[], int collana[]){
    for(int i=0; i<4; i++) // seleziona la pietra
        if((pos==0 || valida(collana[pos-1], i)) && pietre[i]>0){
            // la pietra soddisfa le condizioni
            pietre[i]--;
            collana[pos] = i;

            if(pos+1 > *max ){
                *max = pos+1;
            }

            funz(pos+1, max, n, pietre, collana);
            
            pietre[i]++;
        }
}

int singleRun(FILE* fin){
    int pietre[4], nPietre=0, n, tmp, *collana;
    fscanf(fin, " %d %d %d %d ", &pietre[zaffiro], &pietre[rubino], &pietre[topazio], &pietre[smeraldo]);
    for(n = 0; n < 4; n++) nPietre+= pietre[n]; // somma quante pietre ho in tutto
    if(nPietre == 0) return 0;

    for(tmp = min(pietre), n=0; n < 4; n++)
        pietre[n]-=tmp;

    n = 4*tmp;

    nPietre-= n;
    collana = malloc(sizeof(int) * nPietre);

    tmp = 0;
    funz(0, &tmp, nPietre, pietre, collana);

    free(collana);
    return n+tmp;

}

int main(int argc, char* argv){
    int n, i, val;
    time_t t, tot = clock();
    FILE *fin;

    if ((fin = fopen(FILE_IN, "r+")) == NULL){ printf("Errore nell'apertura del file: \"%s\"", FILE_IN); exit(-1);}
    fscanf(fin, " %d ", &n);

    for(int i = 0; i < n; i++){
        t = clock();
        val = singleRun(fin);
        printf("Test: %3d-> lunghezza massima: %4d\ttime: %f\n", i+1, val, ((double) (clock() - t)) / CLOCKS_PER_SEC);
    }

    fclose(fin);
    printf("Tempo impiegato: %f", ((double) (clock() - tot)) / CLOCKS_PER_SEC);
    
    return 0;
}
