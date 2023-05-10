/*
 * Riccardo Rosin 284211
 * Lab 6 Es 2
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define FILE_IN /* "very_very_easy_test_set.txt" *//* "very_easy_test_set.txt" *//* "easy_test_set.txt" */"hard_test_set.txt"

/*
 * - uno zaffiro deve essere seguito immediatamente o da un altro zaffiro o da un rubino
 * - un  rubino deve essere seguito immediatamente o da uno smeraldo o da un topazio
 * - un  topazio deve essere seguito immediatamente o da uno zaffiro o da un rubino
 * - uno smeraldo deve essere seguito immediatamente o da un altro smeraldo o da un topazio
*/

typedef int***** int5;
enum gemme{zaffiro, rubino, topazio, smeraldo};
typedef enum {false, true} bool;

int funz(int pos, int n, int* pietre, int* notPietre, int* collana, int5 memo);

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

int inMemo(int* notPietre, int5 memo, int val){
    return memo[notPietre[0]][notPietre[1]][notPietre[2]][notPietre[3]][val];
}

void insInMemo(int* notPietre, int5 memo, int i, int ins){
    memo[notPietre[0]][notPietre[1]][notPietre[2]][notPietre[3]][i] = ins;
}

void stampaMemo(int5 memo, int* pietre, int* notPietre){
    int i, j, k, l;
    for(i=0; i<=pietre[0]+notPietre[0]; i++)
        for(j=0; j<=pietre[1]+notPietre[1]; j++)
            for(k=0; k<=pietre[2]+notPietre[2]; k++)
                for(l=0; l<=pietre[3]+notPietre[3]; l++)
                    printf("z:%2d, r:%2d, t:%2d, s:%2d -> %3d %3d %3d %3d\n",
                        i, j, k, l,
                        memo[i][j][k][l][1], memo[i][j][k][l][1], memo[i][j][k][l][2], memo[i][j][k][l][3]
                    );
    printf("\n");
}

int massimo(int a, int b){
    return a>b ? a : b;
}

int fZ(int pos, int n, int* pietre, int* notPietre, int* collana, int5 memo){
    return funz(pos, n, pietre, notPietre, collana, memo);
}

int fR(int pos, int n, int* pietre, int* notPietre, int* collana, int5 memo){
    return funz(pos, n, pietre, notPietre, collana, memo);
}

int fT(int pos, int n, int* pietre, int* notPietre, int* collana, int5 memo){
    return funz(pos, n, pietre, notPietre, collana, memo);
}
 
int fS(int pos, int n, int* pietre, int* notPietre, int* collana, int5 memo){
    return funz(pos, n, pietre, notPietre, collana, memo);
}

// collana vista come gemma + collana (già calcolata)
int funz(int pos, int n, int* pietre, int* notPietre, int* collana, int5 memo){
    int i, r, max = 0;

    for(i=0; i<4; i++){ // seleziona la pietra
        if((pos!=0 && !valida(collana[pos-1], i)) || pietre[i]==0) continue;
        // sto insenrendo la pietra i, voglio vedere quanto è lunga la collana con #'pietre' che parta con 'i'

        pietre[i]--;
        notPietre[i]++;
        collana[pos] = i;

        if((r = inMemo(pietre, memo, i)) == -1){
            // r = funz(pos+1, n, pietre, notPietre, collana, memo);
            switch(i) {
                case zaffiro:
                    r = fZ(pos+1, n, pietre, notPietre, collana, memo);
                    break;
                case rubino:
                    r = fR(pos+1, n, pietre, notPietre, collana, memo);
                    break;
                case topazio:
                    r = fT(pos+1, n, pietre, notPietre, collana, memo);
                    break;
                case smeraldo:
                    r = fS(pos+1, n, pietre, notPietre, collana, memo);
                    break;
            }
            insInMemo(pietre, memo, i, r);
        }
        max = massimo(max, ++r);

        pietre[i]++;
        notPietre[i]--;

    }
    return max;
}

void alloc5(int****** memo, int* pietre){
    int i, j, k, l, m;
    (*memo) = (int5) malloc(sizeof(int****) * (pietre[0]+1));
    for(i=0; i<=pietre[0]; i++){
        (*memo)[i] = (int****) malloc(sizeof(int***) * (pietre[1]+1));
        for(j=0; j<=pietre[1]; j++){
            (*memo)[i][j] = (int***) malloc(sizeof(int**) * (pietre[2]+1));
            for(k=0; k<=pietre[2]; k++){
                (*memo)[i][j][k] = (int**) malloc(sizeof(int*) * (pietre[3]+1));
                for(l=0; l<=pietre[3]; l++){
                    (*memo)[i][j][k][l] = (int*) malloc(sizeof(int) * 4);
                    for(m=0; m<4; m++)
                        (*memo)[i][j][k][l][m] = -1;
                }
            }
        }
    }
}

void free5(int5 memo, int* pietre){
    int i, j, k, l;
    for(i=0; i<=pietre[0]; free(memo[i]), i++)
        for(j=0; j<=pietre[1]; free(memo[i][j]), j++)
            for(k=0; k<=pietre[2]; free(memo[i][j][k]), k++)
                for(l=0; l<=pietre[3]; free(memo[i][j][k][l]), l++);
    free(memo);
}

int singleRun(FILE* fin){
    int pietre[4], notPietre[4] = {0}, nPietre=0, n, *collana, *****memo;
    fscanf(fin, " %d %d %d %d ", &pietre[zaffiro], &pietre[rubino], &pietre[topazio], &pietre[smeraldo]);
    alloc5(&memo, pietre);
    for(n = 0; n < 4; n++) nPietre+= pietre[n]; // somma quante pietre ho in tutto
    if(nPietre == 0) return 0;

    collana = malloc(sizeof(int) * nPietre);

    n = 0;
    n = funz(0, nPietre, pietre, notPietre, collana, memo);
    // stampaMemo(memo, pietre, notPietre);

    free5(memo, pietre);
    free(collana);
    return n;

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
