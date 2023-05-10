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

bool isInOrderI(att* v, int dim){
    if(dim<=1) return true;

    int i, prev;
    prev = v->inizio;
    for(i=1; i<dim; i++){
        if(prev > v[i].inizio) return false;
        prev = v[i].inizio;
    }
    return true;
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

int compare_i(const void *a1p, const void *a2p) {
    att a1 = *((att*) a1p), a2 = *((att*) a2p);
    if(a1.inizio == a2.inizio)
        return 0;
    else if(a1.inizio > a2.inizio)
        return 1;
    return -1;
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

int massimo(att* v, int* opt, int i){
    // cerco il massimo tra gli elementi 0 e k confrontando con la posizone i
    int j, max = 0;
    for (j = 1; j < i; j++)
        if(v[i].inizio >= v[j].fine && opt[j] > opt[max]) max = j;
    return max;
}

void attSel(att *v, const int dim){ 
    int* opt = (int*) malloc(sizeof(int) * dim);
    int* indexs = (int*) malloc(sizeof(int) * dim);
    int maxDim=0, maxLen=0;
    int i, k, max;

    opt[0] = 0;
    indexs[0] = -1;
    for(i=1; i<dim; i++){
        max = massimo(v, opt, i);
        opt[i] = durata(v[i]) + opt[max];
        indexs[i] = max;
    }

    
    for(i=0; i<dim; i++) printf("%3d\t", i);
    printf("\n");
    for(i=0; i<dim; i++) printf("%3d\t", opt[i]);
    printf("\n");
    for(i=0; i<dim; i++) printf("%3d\t", indexs[i]);
    printf("\n");


    max = 0;
    for(i=1; i<dim; i++)
        if(opt[i] > opt[max]) max = i;
    
    printf("Indice del massimo: %d\n", max);
    printf("Durata massima: %d\n", opt[max]);


    stampa_rico(max, indexs, v);

    free(opt);
    free(indexs);
}

void attSelFin(att *v, const int dim){
    int* opt = (int*) malloc(sizeof(int) * dim);
    int* indexs = (int*) malloc(sizeof(int) * dim);
    int maxDim=0, maxLen=0;
    int i, k, max;

    opt[0] = 0;
    indexs[0] = -1;
    for(i=1; i<dim; i++){
        printf("i=%d\na: ", i);
        for(k=i-1; k>=0; k--){
            // devo trovare il primo compatibile
            printf("%d\t", k);
            if(v[k].fine <= v[i].inizio) break;
        }
        max = k;
        printf("\nb: ");
        for(k--; k>=0 &&  indexs[k] == indexs[max]; k--)
            printf("%d\t", k);
        printf("\n1)max: %d; k:%d", max, k);
        max = k;
        if(k==-1) max = 0;
        printf("\n2)max:%d; k:%d", max, k);
        // ho in max la posizione dell'ultimo massimo

        if(opt[i-1] >= durata(v[i]) + opt[max]){
            opt[i] = opt[i-1];
            indexs[i] = -1;
        }
        else{
            opt[i] = durata(v[i]) + opt[max];
            indexs[i] = max;
        }
        printf("\n\n");
    }

    printf("\n");
    printf("\n");

    for(i=0; i<dim; i++) printf("%3d\t", i);
    printf("\n");
    for(i=0; i<dim; i++) printf("%3d\t", opt[i]);
    printf("\n");
    for(i=0; i<dim; i++) printf("%3d\t", indexs[i]);
    printf("\n");


    for(i=dim-1; opt[i] == opt[dim-1]; i--);
    max = i+1;

    printf("Indice del massimo: %d\n", max);
    printf("Durata massima: %d\n", opt[max]);


    stampa_rico(max, indexs, v);

    free(opt);
    free(indexs);
}

int main(int argc, char* argv[]){
    int dim;
    att* v;

    init(&v, &dim);

    // if(!isInOrderF(v, dim))
    //     quickSort(v, 0,  dim-1);
    // printf("Ordinamento per chiave iniziale:\n");
    // qsort(&v[1], dim, sizeof(att), compare_i);
    // stampa_tutto(v, dim);
    
    // attSel(v, dim);

    // printf("\n\n\n");
    printf("Ordinamento per chiave finale:\n");
    qsort(&v[1], dim-1, sizeof(att), compare_f);
    stampa_tutto(v, dim);
    
    attSelFin(v, dim);

    free(v);
    return 0;
}