#ifndef __GRAFO_H
#define __GRAFO_H

#include <stdio.h>
#include "ST.h"

#define MAXC 30
#define ALPHA 0.5



typedef struct edge {int v; int w; int wt;} Edge;
typedef struct graph *Graph;

void G_stampaMatrice(Graph G);

void GRAPH_initM(Graph G);
void GRAPH_freeM(Graph G);
int GRAPH_getIndex(Graph G, char *label);
void GRAPH_edges(Graph G, Edge *a);


void GRAPH_initL(Graph G);
void GRAPH_freeL(Graph G);
void GRAPH_edgesL(Graph G, Edge *a);


void GRAPH_convert(Graph G);


Graph GRAPH_init(int V);
void GRAPH_readM(Graph G, FILE* fp);
void GRAPH_free(Graph G);
void GRAPH_show(Graph G);

int GRAPH_isSubgraph(Graph G, Item A, Item B, Item C);


#endif