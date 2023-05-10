#include <stdio.h>
#include <stdlib.h>
#define Z 19
#define R 6
#define T 8
#define S 5
#define MAX (Z + R + T + S)

typedef enum {zaffiro, rubino, topazio, smeraldo} Pietra;

int disp_rip(int pos, int*val, int *sol, int *inventario, int n, int k, int lunghezzaMax, int trovato);
int maggiore(int a, int b);
int compatibile(Pietra a, Pietra b);

int main(){
    int lunghezza=0;
    int val[4]={zaffiro, rubino, topazio, smeraldo};
    int inventario[4]={Z,R,T,S};
    int sol[MAX];
    for(int i=0;i<MAX;i++) sol[i]=-1;

    lunghezza = disp_rip(0, val, sol, inventario, 4, MAX, 0, 0);
    printf("lunghezza: %d\n", lunghezza);
    return 0;
}

int disp_rip(int pos, int*val, int *sol, int *inventario, int n, int k, int lunghezzaMax, int trovato){
    int i;
    if(pos+1>=n){ //guardando la soluzione dice che � 24 quindi per ora in mancanza di idee su come terminare uso questo
        // for(i=0;i<k && sol[i]!=-1;i++)
        //     printf("%d ", sol[i]);
        // printf("\n");

        return maggiore(lunghezzaMax, pos+1);
    }

    for(i=0;i<n;i++){
        
        if(pos==0){
            sol[pos]=val[i];
            inventario[i]--;
            lunghezzaMax=disp_rip(pos+1, val, sol, inventario, n, k, lunghezzaMax, trovato);
            inventario[i]++;
        
        
        }else{
            if(compatibile(sol[pos-1],val[i])==1){
                if(inventario[i]==0){ //se val[i]==zaffiro allora inventario[i] � quello degli zaffiri
                    lunghezzaMax=maggiore(lunghezzaMax, pos+1);
                    continue;
                }else{
                    sol[pos]=val[i];
                    inventario[i]--;
                    lunghezzaMax=disp_rip(pos+1, val, sol, inventario, n, k, lunghezzaMax, trovato);
                    inventario[i]++;
                }
            }else{
                lunghezzaMax=maggiore(lunghezzaMax, pos+1);
                continue;
            }
        }
    }
    return maggiore(lunghezzaMax, pos);
}

int maggiore(int a, int b){
    if(a>b)
        return a;
    return b;
}

int compatibile(Pietra a, Pietra b){ //mi dice se b pu� essere dopo a
    if(a==zaffiro || a==topazio)
    {
        if(b==zaffiro || b==rubino)
            return 1;
        return -1;
    }
    if(a==smeraldo || a==rubino)
    {
        if(b==smeraldo || b==topazio)
            return 1;
    }
    return -1;
}
