#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include "ST.h"
#include "item.h"

typedef enum {false, true} Bool;

typedef struct node{
    int v;
    int wt;
    struct node *next;
    Bool lock;
} *link;

struct graph{
    int V;
    int E;
    link* ladj;
    ST tab;
    link z;
    unsigned int sizeSet;
};

struct archi_s{
    int T;
    int B;
    int F;
    int C;
};

typedef struct edgeIArray{
    Edge* array;
    int size;
    int wt;
} EdgeIArray;

struct edgeIArrayWrapper{
    EdgeIArray* array;
    int size;
    int allocatedDim;
    int indexMax;
};

struct individuazioneParams{
    Graph G;
    Edge* listE;
    Edge* tmp;
    EdgeIArrayWrapper result;
    int index;
    int maxIt;
};

// generici
static Edge EDGE_create(int v, int w, int wt) {
    Edge e;
    e.v = v; e.w = w; e.wt = wt;
    return e;
}
static EdgeI EDGEI_create(Graph G, Edge in) {
    EdgeI e;
    e.v = ST_searchByIndex(G->tab, in.v); e.w = ST_searchByIndex(G->tab, in.w); e.wt = in.wt;
    return e;
}
static link NEW(int v, int wt, link next) {
    link x = malloc(sizeof(*x));
    x->v = v;
    x->wt = wt;
    x->next = next;
    x->lock = false;
    return x;
}
static void insertE(Graph G, Edge e);
static void removeE(Graph G, Edge e);



Graph GRAPH_initL(int V) {
    int v;
    Graph G = malloc(sizeof(*G));
    G->V = V;
    G->E = 0;
    G->tab = ST_init(G->V, ALPHA);
    G->z = NEW(-1, -1, NULL);
    G->ladj = malloc(ST_sizeSet(G->tab)*sizeof(link));
    for (v = 0; v < ST_sizeSet(G->tab); v++)
        G->ladj[v] = G->z;
    return G;
}

Graph GRAPH_load(FILE* fin){
    int V, i, id1, id2, wt;

    Item item1, item2;
    Graph G;
    fscanf(fin, "%d", &V);
    G = GRAPH_initL(V);

    for(i=0; i<V; i++){
        item1 = ITEM_scan(fin);
        #if DBG
        printf("%2d\t", ST_insert(G->tab, item1));
        #else
        ST_insert(G->tab, item1);
        #endif
    }
    printf("\n");
    while(
        ITEM_checkNull((item1 = ITEM_scan(fin))) == false &&
        ITEM_checkNull((item2 = ITEM_scan(fin))) == false &&
        fscanf(fin, "%d", &wt) == 1
    ){
        id1 = ST_searchId(G->tab, KEY_get(&item1));
        id2 = ST_searchId(G->tab, KEY_get(&item2));
        if(id1>=0 && id2>=0) insertE(G, EDGE_create(id1, id2, wt));
    }
    return G;
}

Edge* GRAPH_edges(Graph G) {
    int v, i=0;
    Edge *a = malloc(G->E * sizeof(Edge));
    link t;
    for (v=0; v < ST_sizeSet(G->tab); v++)
        for (t=G->ladj[v]; t != G->z; t = t->next)
            if (v < t->v) a[i++] = EDGE_create(v, t->v, t->wt);
    return a;
}

void GRAPH_insertE(Graph G, int id1, int id2, int wt){
    insertE(G, EDGE_create(id1, id2, wt));
}

static void insertE(Graph G, Edge e) {
    int v = e.v, w = e.w, wt = e.wt;
    G->ladj[v] = NEW(w, wt, G->ladj[v]);
    G->E++;
}

void GRAPH_removeE(Graph G, int id1, int id2){
    removeE(G, EDGE_create(id1, id2, 0));
}

static void removeE(Graph G, Edge e) {
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

static void GRAPH_freeL(Graph G) {
    int v;
    link t, next;
    for (v=0; v < ST_sizeSet(G->tab); v++)
        for (t=G->ladj[v]; t != G->z; t = next) {
            next = t->next;
            free(t);
        }
    free(G->ladj);
    free(G->z);
}

void GRAPH_free(Graph G){
    GRAPH_freeL(G);
    ST_free(G->tab);
    free(G);
}

static void dfsR(Graph G, Edge e, int *time, struct archi_s* archi, int *pre, int *post, int *st){
    link t;
    int v, w = e.w;
    Edge x;
    if (e.v != e.w){
        // printf("(%s, %s): T\n", STsearchByIndex(G->tab, e.v), STsearchByIndex(G->tab, e.w));
        archi->T++;
    }
    st[e.w] = e.v;
    pre[w] = (*time)++;
    for (t = G->ladj[w]; t != G->z; t = t->next)
        if(t->lock){
            continue; // salta l'arco perché rimosso 
        }else if (pre[t->v] == -1)
            dfsR(G, EDGE_create(w, t->v, 0), time, archi, pre, post, st);
        else {
            v = t->v;
            x = EDGE_create(w, v, 0);
            if (post[v] == -1){
                // printf("(%s, %s): B\n", STsearchByIndex(G->tab, x.v), STsearchByIndex(G->tab, x.w));
                archi->B++;
            }else if (pre[v] > pre[w]){
                // printf("(%s, %s): F\n", STsearchByIndex(G->tab, x.v), STsearchByIndex(G->tab, x.w));
                archi->F++;
            }else{
                // printf("(%s, %s): C\n", STsearchByIndex(G->tab, x.v), STsearchByIndex(G->tab, x.w));
                archi->C++;
            }
        }
    
    post[w] = (*time)++;
}

int GRAPH_dfs(Graph G, Item var) {
    int v, time=0, *pre, *post, *st, id = ST_searchId(G->tab, KEY_get(&var));
    if(id == -1) return -1;
    Item* items = ST_getArray(G->tab);
    pre = malloc(G->V * sizeof(int));
    post = malloc(G->V * sizeof(int));
    st = malloc(G->V * sizeof(int));

    struct archi_s archi = {0, 0, 0, 0};

    for (v=0; v < G->V; v++) {  // inizializzazione dei vettori
        pre[v]=-1;
        post[v]=-1;
        st[v]=-1;
    }

    // chiamo una volta dal nodo passato
    dfsR(G, EDGE_create(id,id, 0), &time, &archi, pre, post, st);

    for (v=0; v < G->V; v++) // chiamo per tutti i nodi che non sono stati raggiunti
        if (pre[ST_searchId(G->tab, KEY_get(&items[v]))] ==-1)
            dfsR(G, EDGE_create(v, v, 0), &time, &archi, pre, post, st);
    
    free(pre); free(post); free(st);
    free(items);
    // printf("T: %d, F: %d, C: %d, B: %d\n", archi.T, archi.F, archi.C, archi.B);
    return archi.B;
}

int GRAPH_isDAG(Graph G){
    Item item = ST_getOneItem(G->tab);
    return GRAPH_dfs(G, item) == 0;
}

static int numberBArch(Graph G){
    Item item = ST_getOneItem(G->tab);
    return GRAPH_dfs(G, item);
}

static int invertLockEdges(Graph G, Edge e){
    link x;

    for(x = G->ladj[e.v]; x != G->z; x = x->next)
        if(x->v == e.w)
            return (x->lock = (x->lock) ? false : true);
    return -1;
}

static int getLock(Graph G, Edge e){
    int i=e.v;

    for(link x = G->ladj[i]; x != G->z; x = x->next)
        if(x->v == e.w)
            return x->lock;
    return -1;
}

static void individuazioneR(struct individuazioneParams arg){

    if(GRAPH_isDAG(arg.G)) { // se il grafo è aciclico    
        if(arg.result->allocatedDim == arg.result->size){    // riallocazione memoria logaritmica
            arg.result->allocatedDim *= 2;
            arg.result->array = realloc(arg.result->array, arg.result->allocatedDim * sizeof(EdgeIArray));
        }
        arg.result->array[arg.result->size].array = malloc(arg.index * sizeof(Edge)); // alloco l'insieme di archi
        arg.result->array[arg.result->size].size = arg.index;
        int wt = 0;

        for(int i=0; i < arg.index; i++){   // copio i valori
            // arg.result->array[arg.result->size].array[i].v = ST_searchByIndex(arg.G->tab, arg.tmp[i].v);
            // arg.result->array[arg.result->size].array[i].w = ST_searchByIndex(arg.G->tab, arg.tmp[i].w);
            arg.result->array[arg.result->size].array[i].v = arg.tmp[i].v;
            arg.result->array[arg.result->size].array[i].w = arg.tmp[i].w;
            wt+= (arg.result->array[arg.result->size].array[i].wt = arg.tmp[i].wt);
        }
        arg.result->array[arg.result->size].wt = wt;
        if(arg.result->indexMax == -1 || arg.result->array[arg.result->indexMax].wt < wt) arg.result->indexMax = arg.result->size;
        (arg.result->size)++;

        return;
    }    


    for(int i=0; i<arg.G->V; i++){  // per ogni arco simulo la rimozione
        if(getLock(arg.G, arg.listE[i]) == 1) continue;
        
        invertLockEdges(arg.G, arg.listE[i]);
        ("%d\n", getLock(arg.G, arg.listE[i]));
        arg.tmp[arg.index] = arg.listE[i];
        arg.index++;

        individuazioneR(arg);
        
        arg.index--;
        invertLockEdges(arg.G, arg.listE[i]);
    }

}

EdgeIArrayWrapper GRAPH_individuazione(Graph G){
    int i, j, n = numberBArch(G);
    if(n == 0) exit(-1);
    Edge* listE = GRAPH_edges(G);
    Edge* tmp = malloc(n * sizeof(Edge));
    
    EdgeIArrayWrapper result = malloc(sizeof(*result));
    result->size = 0;
    result->allocatedDim = 1;
    result->array = malloc(sizeof(EdgeIArray));
    result->indexMax = -1;

    struct individuazioneParams var = {
        .G = G,
        .listE = listE,
        .tmp = tmp,
        .result = result,
        .index = 0,
        .maxIt = G->E,
    };
    individuazioneR(var);
    printf("Ciao");

    result->allocatedDim = result->size;
    result->array = realloc(result->array, result->allocatedDim * sizeof(EdgeIArray));

    for(i = 0; i < result->size; i++){
        printf("n: %2d, wt: %4d: ", result->array[i].size, result->array[i].wt);
        for(j=0; j<result->array[i].size; j++){
            ITEM_show(ST_searchByIndex(G->tab, result->array[i].array[j].v));
            printf(" ");
            ITEM_show(ST_searchByIndex(G->tab, result->array[i].array[j].w));
            printf(" | ");
        }
        printf("\n");
    }

    free(listE);
    free(tmp);
    return result;
}

void GRAPH_setDag(Graph G, EdgeIArrayWrapper result){
    int i;
    printf("Inposto il DAG rimuovendo gli archi a peso massimo\n");
    EdgeIArray arr = result->array[result->indexMax];
    for(i=0; i<arr.size; i++)
        invertLockEdges(G, arr.array[i]);

}

void GRAPH_freeEdgeIArrayWrapper(EdgeIArrayWrapper e){
    for(int i=0; i<e->size; i++)
        free(e->array[i].array);
    free(e->array);
    free(e);
}