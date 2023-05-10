/*
 * Riccardo Rosin 284211
 * Lab 4 Es 3
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// #include<unistdat.h>  //Header file for sleep()-> man 3 sleep for details->
#include<pthread.h>
#define FILE_IN /* "very_very_easy_test_set.txt" *//* "very_easy_test_set.txt" *//* "easy_test_set.txt" */ "hard_test_set.txt"


/*
 * - uno zaffiro deve essere seguito immediatamente o da un altro zaffiro o da un rubino
 * - uno smeraldo deve essere seguito immediatamente o da un altro smeraldo o da un topazio
 * - un rubino deve essere seguito immediatamente o da uno smeraldo o da un topazio
 * - un topazio deve essere seguito immediatamente o da uno zaffiro o da un rubino
*/
enum gemme{zaffiro, rubino, topazio, smeraldo};
typedef enum {false, true} bool;
typedef struct{
    int pos;
    int* max;
    int n;
    int* pietre;
    int* collana;
} dato;

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

void stampaCollana(int* collana, int n){
    char str[] = "zrts";
    for(int i=0; i<n; i++) printf("%c", str[collana[i]]);
    if(n==0) printf("-");
}

void* funz(void* d){
    int i;
    dato* _d = (dato*) d;

    if(_d->pos == _d->n) return NULL;
    
    for(i=0; i<4; i++){ // seleziona la pietra
        if(_d->pos==0 || (valida(_d->collana[_d->pos-1], i) && _d->pietre[i]>0)){
            if(_d->pos+1 > *_d->max ) *_d->max = _d->pos+1;
            
            _d->pietre[i]--;
            _d->collana[_d->pos] = i;
            _d->pos++;
            
            funz((void*) _d);
            
            _d->pietre[i]++;
            _d->pos--;
        }
    }

}

int singleRun(FILE* fin){
    int pietre[4], nPietre=0, n, i;
    dato dati[4];
    pthread_t thread_id[4];
    fscanf(fin, " %d %d %d %d ", &pietre[zaffiro], &pietre[rubino], &pietre[topazio], &pietre[smeraldo]);
    for(n = 0; n < 4; n++) nPietre+= pietre[n]; // somma quante pietre ho in tutto
    if(nPietre == 0) return 0;

    n = 0;
    for(i=0; i<4; i++){
        dati[i].pos = 1;
        dati[i].max = &n;
        dati[i].n = nPietre;
        dati[i].pietre = (int*) malloc(sizeof(int) * 4);
        dati[i].collana = (int*) malloc(nPietre * sizeof(int));
        
        memcpy(dati[i].pietre, pietre, 4*sizeof(int));

        dati[i].pietre[i]--;
        dati[i].collana[0] = i;
        
        pthread_create(&thread_id[i], NULL, funz, (void*) &dati[i]);
    }

    for(i=0; i<4; i++){
        pthread_join(thread_id[i], NULL);
        free(dati[i].pietre);
        free(dati[i].collana);
    }
    
    return n;

}

int main(int argc, char* argv[]){
    int n, i;

    FILE *fin;
    if ((fin = fopen(FILE_IN, "r+")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", FILE_IN);
        exit(-1);
    }
    fscanf(fin, " %d ", &n);
    
    for(int i = 0; i < n; i++){
        printf("Test: %3.d-> lunghezza massima: %d\n", i+1, singleRun(fin));
    }

    fclose(fin);
    
    return 0;
}
