#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include "ST.h"

typedef struct node{
    int v;
    int wt;
    struct node *next;
} *link;
struct graph{
    int V;
    int E;
    int **madj;
    link* ladj;
    ST tab;
    link z;
    short int mat;
    short int list;
};



// generici
static Edge EDGE_create(int v, int w, int wt) {
    Edge e;
    e.v = v; e.w = w; e.wt = wt;
    return e;
}




// matrice delle adiacenze
static int** MATRIXint(int r, int c, int val) {
    int i, j;
    int **t = malloc(r * sizeof(int *));
    for (i=0; i < r; i++) t[i] = malloc(c * sizeof(int));
    for (i=0; i < r; i++)
        for (j=0; j < c; j++)
            t[i][j] = val;
    return t;
}

void GRAPH_initM(Graph G) {
    G->mat = 1;
    G->madj = MATRIXint(ST_sizeSet(G->tab), ST_sizeSet(G->tab), 0);
}

void G_stampaMatrice(Graph G){
    int i, j;

    printf("  | ");
    for(i=1; i <ST_sizeSet(G->tab); i++){
        printf("%3d ", i-1);
    }
    printf("\n---|-");
    for(i=1; i<ST_sizeSet(G->tab); i++){
        printf("----");
    }
    printf("\n");
    for(i=0; i<ST_sizeSet(G->tab); i++){
        printf("%2d | ", i);
        for(j=0; j<ST_sizeSet(G->tab); j++){
            printf("%3d ", G->madj[i][j]);
        }
        printf("\n");
    }
}

void GRAPH_freeM(Graph G) {
    int i;
    for (i=0; i<ST_sizeSet(G->tab); i++)
        free(G->madj[i]);
    free(G->madj);
}

static void insertEM(Graph G, Edge e) {
    int v = e.v, w = e.w, wt =e.wt;
    if (G->madj[v][w] == 0)
        G->E++;
    G->madj[v][w] = wt;
    G->madj[w][v] = wt;
}

int GRAPH_getIndex(Graph G, char *label) {
    Item id = ST_searchItem(G->tab, label);
    if (ITEM_checkNull(id)) {
        ST_insert(G->tab, id);
    }
    // return ITEM_getValue(id);
}

static void removeEM(Graph G, Edge e) {
    int v = e.v, w = e.w;
    if (G->madj[v][w] != 0)
        G->E--;
    G->madj[v][w] = 0;
    G->madj[w][v] = 0;
}

void GRAPH_edges(Graph G, Edge *a) {
    int v, w, E = 0;
    for (v=0; v < ST_sizeSet(G->tab); v++)
        for (w=v+1; w < ST_sizeSet(G->tab); w++)
        if (G->madj[v][w] !=0)
            a[E++] = EDGE_create(v, w, G->madj[v][w]);
    return;
}




// lista delle adiacenze
static link NEW(int v, int wt, link next) {
    link x = malloc(sizeof(*x));
    x->v = v;
    x->wt = wt;
    x->next = next;
    return x;
}

void GRAPH_initL(Graph G) {
    G->list = 1;
    int v;
    G->z = NEW(-1, -1, NULL);
    G->ladj = malloc(ST_sizeSet(G->tab)*sizeof(link));
    for (v = 0; v < ST_sizeSet(G->tab); v++)
        G->ladj[v] = G->z;
}

void GRAPH_freeL(Graph G) {
    int v;
    link t, next;
    for (v=0; v < ST_sizeSet(G->tab); v++)
        for (t=G->ladj[v]; t != G->z; t = next) {
            next = t->next;
            free(t);
        }
    free(G->ladj); free(G->z);
}

void GRAPH_edgesL(Graph G, Edge *a) {
    int v, E = 0;
    link t;
    for (v=0; v < ST_sizeSet(G->tab); v++)
        for (t=G->ladj[v]; t != G->z; t = t->next)
            if (v < t->v) a[E++] = EDGE_create(v, t->v, t->wt);
}

static void insertEL(Graph G, Edge e) {
    int v = e.v, w = e.w, wt = e.wt;
    G->ladj[v] = NEW(w, wt, G->ladj[v]);
    G->ladj[w] = NEW(v, wt, G->ladj[w]);
    G->E++;
}

static void removeEL(Graph G, Edge e) {
    int v = e.v, w = e.w; link x, p;
    for (x = G->ladj[v], p = NULL; x != G->z; p = x, x = x->next) {
        if (x->v == w) {
            if (x == G->ladj[v]) G->ladj[v] = x->next;
            else p->next = x->next;
            break;
        }
    }
    for (x = G->ladj[w], p = NULL; x != G->z; p = x, x = x->next) {
        if (x->v == v) {
            if (x == G->ladj[w]) G->ladj[w] = x->next;
            else p->next = x->next;
            break;
        }
    }
    G->E--; free(x);
}


// conversione
static void GRAPH_convertM(Graph G){
    int i, j;
    for(i=0; i<ST_sizeSet(G->tab); i++){
        for(j=i+1; j<ST_sizeSet(G->tab); j++){
            if(G->madj[i][j] != 0){
                insertEL(G, EDGE_create(i, j, G->madj[i][j]));
            }
        }
    }
}

void GRAPH_convert(Graph G){
    if(G->mat && !G->list){ GRAPH_initL(G); GRAPH_convertM(G);}
    if(G->list && !G->mat){ /* GRAPH_initM(G); GRAPH_convertL(G); */ ;}
}

// generic

Graph GRAPH_init(int V){
    Graph G = malloc(sizeof *G);
    G->V = V;
    G->E = 0;
    G->mat = 0;
    G->list = 0;
    G->tab = ST_init(G->V, ALPHA);
    return G;
}

void GRAPH_readM(Graph G, FILE* fp){
    if(!G->mat) GRAPH_initM(G);
    Item id1, id2;
    int wt, i, code1, code2;

    for(i=0; i<G->V; i++){
        id1 = ITEM_scan(fp);
        id2 = ITEM_scan(fp);
        fscanf(fp, "%d ", &wt);

        if(( code1 = ST_searchId(G->tab, KEY_get(&id1)) ) == -1)
            code1 = ST_insert(G->tab, id1);
        if(( code2 = ST_searchId(G->tab, KEY_get(&id2)) ) == -1)
            code2 = ST_insert(G->tab, id2);
        
        if(G->mat) insertEM(G, EDGE_create(code1, code2, wt));

    }
}

void GRAPH_free(Graph G){
    if (G->mat) GRAPH_freeM(G);
    if (G->list) GRAPH_freeL(G);
    ST_free(G->tab);
    free(G);
}

static int cmp(const void* a, const void* b){
    Item* x = (Item*) a;
    Item* y = (Item*) b;
    Key k1 = KEY_get(x);
    Key k2 = KEY_get(y);
    int i = KEY_cmp(k1, k2);
    return i;
}

void GRAPH_show(Graph G){
    int i, j, k, index, V = ST_count(G->tab);
    Item* vet = ST_getArray(G->tab);
    Item* inc = malloc(ST_count(G->tab) * sizeof(Item));
    
    qsort(vet, V, sizeof(Item), cmp);

    for(i=0; i<V; i++){
        KEY_show(KEY_get(&vet[i]));
        
        index = ITEM_getValue(vet[i]);
        k=0;
        
        for(j=0; j<ST_sizeSet(G->tab); j++){
            if(G->madj[index][j] != 0){
                inc[k] = ST_searchByIndex(G->tab, j);
                k++;
            }
        }

        qsort(inc, k, sizeof(Item), cmp);

        for(j=0; j<k; j++){
            printf("\t");
            KEY_show(KEY_get(&inc[j]));
        }
    }


    free(vet);
    free(inc);
}

int GRAPH_isSubgraph(Graph G, Item A, Item B, Item C){
    int i, j;
    Item vet[3];

    A.value = ST_searchId(G->tab, KEY_get(&A));
    B.value = ST_searchId(G->tab, KEY_get(&B));
    C.value = ST_searchId(G->tab, KEY_get(&C));

    vet[0] = A;
    vet[1] = B;
    vet[2] = C;


    for (i=0; i < 3; i++)
        for (j=0; j < 3; j++)
            if (i!=j && G->madj[ITEM_getValue(vet[i])][ITEM_getValue(vet[j])] == 0 )
                return 0;
    return 1;
}





