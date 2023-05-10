/*
 * Riccardo Rosin 284211
 * Lab 6 Es 1
*/
#include<stdio.h>
#include<stdlib.h>
#define FILE_IN "att1.txt"

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
    (*dim)++;
    *v = malloc(*dim * sizeof(att));
    
    (*v)[0].inizio = 0;
    (*v)[0].fine = 0;

    for(int i=1; i<*dim; i++)
        fscanf(fin, " %d %d ",
            &(*v+i)->inizio,
            &(*v+i)->fine
        );

    fclose(fin);
}

bool overlap(att a, att b){
    // do per sontato che a.inizio < b.inizio
    return a.fine > b.inizio;
}

int durata(att a){
    return a.fine - a.inizio;
}
     
void stampa_el(att a){
    printf("\tinizio: %3d\tfine: %3d\tdurata: %3d\n", a.inizio, a.fine, durata(a));
}

void stampa_sequenza(att* v, int* arr,  int dim){
    for(int i=0; i<dim; i++)
        stampa_el(v[arr[i]]);
    printf("\n");
}

void stampa_tutto(att* v, int dim){
    for(int i=0; i<dim; i++)
        printf("%3d\tinizio: %3d\tfine: %3d\tdurata: %3d\n", i, v[i].inizio, v[i].fine, durata(v[i]));
    printf("\n");
}

bool isInOrderF(att* v, int dim){
    if(dim<=1) return true;

    int i, prev;
    prev = v->fine;
    for(i=1; i<dim; i++){
        if(prev > v[i].fine) return false;
        prev = v[i].fine;
    }
    return true;
}

int compare_f(const void *a1p, const void *a2p) {
    att a1 = *((att*) a1p), a2 = *((att*) a2p);
    if(a1.fine == a2.fine)
        return 0;
    else if(a1.fine > a2.fine)
        return 1;
    return -1;
}

void stampa_rico(int index, int* indici, att* v){
    if(indici[index] == -1) return;

    if(indici[indici[index]] != -1){
        stampa_rico(indici[index], indici, v);
    }
    stampa_el(v[index]);

}

void attSelFin(att *v, const int dim){
    int* opt = (int*) malloc(sizeof(int) * dim);
    int* indexs = (int*) malloc(sizeof(int) * dim);
    int maxDim=0, maxLen=0;
    int i, k, max;

    opt[0] = 0;
    indexs[0] = -1;
    for(i=1; i<dim; i++){
        for(k=i-1; k>=0; k--){
            // devo trovare il primo compatibile
            if(v[k].fine <= v[i].inizio) break;
        }
        max = k;
        for(k--; k>=0 &&  indexs[k] == indexs[max]; k--);
        max = k;
        if(k==-1) max = 0;
        // ho in max la posizione dell'ultimo massimo

        if(opt[i-1] >= durata(v[i]) + opt[max]){
            opt[i] = opt[i-1];
            indexs[i] = -1;
        }
        else{
            opt[i] = durata(v[i]) + opt[max];
            indexs[i] = max;
        }
    }

    for(i=dim-1; opt[i] == opt[dim-1]; i--);
    max = i+1;
    printf("Durata massima: %d\n", opt[max]);


    stampa_rico(max, indexs, v);

    free(opt);
    free(indexs);
}

int main(int argc, char* argv[]){
    int dim;
    att* v;

    init(&v, &dim);

    qsort(&v[1], dim-1, sizeof(att), compare_f);
    
    attSelFin(v, dim);

    free(v);
    return 0;
}