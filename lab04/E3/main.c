/*
 * Riccardo Rosin 284211
 * Lab 4 Es 3
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define FILE_IN /* "very_very_easy_test_set.txt" *//* "very_easy_test_set.txt" */"easy_test_set.txt"/* "hard_test_set.txt" */
#define DICO 0


/*
 * - uno zaffiro deve essere seguito immediatamente o da un altro zaffiro o da un rubino
 * - uno smeraldo deve essere seguito immediatamente o da un altro smeraldo o da un topazio
 * - un rubino deve essere seguito immediatamente o da uno smeraldo o da un topazio
 * - un topazio deve essere seguito immediatamente o da uno zaffiro o da un rubino
*/
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

bool rico(int pos, int n, int* pietre, int* collana){
    int i, tmp;

    for(i=0; i<4; i++){ // seleziona la pietra
        if((pos==0 || valida(collana[pos-1], i)) && pietre[i]>0){
            // la pietra soddisfa le condizioni
            pietre[i]--;
            collana[pos] = i;

            tmp = rico(pos+1, n, pietre, collana);
            pietre[i]++;

            if(tmp) return true;
        }
    }
    return false;
}

void funz(int pos, int* max, int n, int* pietre, int* collana){
    int i;
    if(pos==n) return;

    for(i=0; i<4; i++){ // seleziona la pietra
        if((pos==0 || valida(collana[pos-1], i)) && pietre[i]>0){
            // la pietra soddisfa le condizioni
            if(pos+1 > *max ) *max = pos+1;
            pietre[i]--;
            collana[pos] = i;

            funz(pos+1, max, n, pietre, collana);
            
            pietre[i]++;
        }
    }
}

int singleRun(FILE* fin){
    int pietre[4], nPietre=0, r, l, n, *collana;
    fscanf(fin, " %d %d %d %d ", &pietre[zaffiro], &pietre[rubino], &pietre[topazio], &pietre[smeraldo]);
    for(n = 0; n < 4; n++) nPietre+= pietre[n]; // somma quante pietre ho in tutto
    if(nPietre == 0) return 0;

    collana = malloc(sizeof(int) * nPietre);

    if(DICO){
        l = 1; r=nPietre+1;
        while(l!=(r-1)){
            n = (r+l)/2;

            if(!rico(0, n, pietre, collana)) r = n;
            else l=n;
        }
    }else{
        l = 0;
        funz(0, &l, nPietre, pietre, collana);
    }

    free(collana);
    return l;

}

int main(int argc, char* argv){
    int n, i, val;
    time_t t, tot = clock();
    FILE *fin;
    if ((fin = fopen(FILE_IN, "r+")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", FILE_IN);
        exit(-1);
    }   
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
    