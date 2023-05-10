/*
 * Riccardo Rosin 284211
 * Lab 3 Es 1
*/
#include<stdio.h>
#include<stdlib.h>

int ricorrenza(int *a, int N, int num){
    // conto quante occorrenze ho avuo della stessa lettera nel sottovettore scelto
    int c=0;
    for(int i=0; i<N; i++) if(a[i]==num) c++;
    return c;
}

// return -1 se non trova, oppure il numero se trova
int majority(   int *a, int N){
    int i=0, d=0, s=0, rd, rs;
    if(N==1) return *a; // se vettore di un solo elemento restituisci il valore effettivo
    i = N/2;
    
    s = majority(a, i); // sinistro
    if(s!=-1) {
        rs = ricorrenza(a, N, s);
        if(rs > i) return s;
    }
    
    d = majority(a+i, N-i); // destro
    if(d!=-1){
        rd = ricorrenza(a, N, d);
        if(rd > i) return d;
    }

    return -1;    
}

int main(int argc, char* argv){
    int vet[] = {2, 0, 2, 0};
    printf("%d\n", majority(vet, sizeof(vet)/sizeof(int)));
    return 0;
}