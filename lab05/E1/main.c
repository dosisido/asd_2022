/*
 * Riccardo Rosin 284211
 * Lab 5 Es 1
*/
#include<stdio.h>
#include<stdlib.h>
#define FILE_IN "att.txt"

typedef enum {false, true} bool;
typedef struct {
    int inizio;
    int fine;
} att;

void init(att** v, int* dim){
    FILE *fin;
    if ((fin = fopen(FILE_IN, "r+")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", FILE_IN);
        exit(-1);
    }
    
    fscanf(fin, " %d ", dim);
    *v = malloc(*dim * sizeof(att));

    for(int i=0; i<*dim; i++)
        fscanf(fin, " %d %d ",
            &(*v+i)->inizio,
            &(*v+i)->fine
        );

    fclose(fin);
}

void print(att* v, int* arr,  int dim){
    for(int i=0; i<dim; i++)
        printf("inizio: %3d\tfine: %3d\n", v[arr[i]].inizio, v[arr[i]].fine);
    printf("\n");
}

void swich_(att* a, att* b){
    att temp;

    temp.inizio = a->inizio;
    temp.fine = a->fine;

    a->inizio = b->inizio;
    a->fine = b->fine;

    b->inizio = temp.inizio;
    b->fine = temp.fine;
}

bool isInOrder(att* v, int dim){
    if(dim<=1) return true;

    int i, prev;
    prev = v->inizio;
    for(i=1; i<dim; i++){
        if(prev > v[i].inizio) return false;
        prev = v->inizio;
    }
    return true;
}

void quickSort(att a[], int primo, int ultimo){
    int i, j, pivot;

    if(primo < ultimo){
        pivot = primo;
        i = primo;
        j = ultimo;     
        
        while(i < j){
            while(a[i].inizio <= a[pivot].inizio && i<ultimo) i++;
            while(a[j].inizio > a[pivot].inizio) j--;
            if(i<j)
                swich_(&a[i], &a[j]);
        }

        swich_(&a[pivot], &a[j]);

        quickSort(a, primo, j-1);
        quickSort(a, j+1, ultimo);
    }
}

bool overlap(att a, att b){
    // do per sontato che a.inizio < b.inizio
    return a.fine > b.inizio;
}

int durata(att a){
    return a.fine - a.inizio;
}

void arrcopy(int* a, int* b, int n){
    for(int i=0; i<n; i++)
        a[i] = b[i];
}

bool rico(int pos, int index, int len, const att *source, const int dim, int* result, int* maxDim, int* maxLen, int* bestResult){
    int i;

    for(i=index; i<dim; i++){
        if(pos==0 || !overlap(source[result[pos-1]], source[i])){
            
            len+= durata(source[i]);
            result[pos] = i;
        
            if(len > *maxLen){
                *maxLen = len;
                *maxDim = pos+1;
                arrcopy(bestResult, result, pos+1);
            }

            rico(pos+1, i+1, len, source, dim, result, maxDim, maxLen, bestResult);

            len-= durata(source[i]);

        }
    }
}

void attSel(att *v, const int dim){
    int* result = (int*) malloc(sizeof(int) * dim);
    int* bestResult = (int*) malloc(sizeof(int) * dim);
    int maxDim=0, maxLen=0;

    rico(0, 0, 0, v, dim, result, &maxDim, &maxLen, bestResult);

    printf("Sequenza massima di %d attivita' di durata %d:\n", maxDim, maxLen);
    print(v, bestResult, maxDim);
    for(int i = 0; i <maxDim; i++) printf("%3d\t", bestResult[i]+1);

    free(result);
    free(bestResult);
}

int main(int argc, char* argv[]){
    int dim;
    att* v;

    init(&v, &dim);

    if(!isInOrder(v, dim))
        quickSort(v, 0,  dim-1); // inutile perché dati forniti ordinati, ma non viene specificato
    attSel(v, dim);

    free(v);
    return 0;
}