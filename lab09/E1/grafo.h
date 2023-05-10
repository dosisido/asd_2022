#ifndef __GRAFO_H
#define __GRAFO_H

#include <stdio.h>
#include "ST.h"
#include "item.h"

#define ALPHA 0.5
#define DBG 0



typedef struct edge {int v; int w; int wt;} Edge;
typedef struct edgeI {Item v; Item w; int wt;} EdgeI;
typedef struct edgeIArrayWrapper* EdgeIArrayWrapper;

typedef struct graph *Graph;


Graph GRAPH_initL(int V);
Graph GRAPH_load(FILE* fin);
Edge* GRAPH_edges(Graph G);
void GRAPH_insertE(Graph G, int id1, int id2, int wt);
void GRAPH_removeE(Graph G, int id1, int id2);
void GRAPH_free(Graph G);
int GRAPH_dfs(Graph G, Item var);
int GRAPH_isDAG(Graph G);
EdgeIArrayWrapper GRAPH_individuazione(Graph G);
void GRAPH_setDag(Graph G, EdgeIArrayWrapper result);
void GRAPH_freeEdgeIArrayWrapper(EdgeIArrayWrapper e);

#endif