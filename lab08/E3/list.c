#include<stdio.h>
#include<stdlib.h>
#include "list.h"

typedef struct node_s{
    Titolo val;
    struct node_s *prev;
    struct node_s *next;
} node, *link;

typedef struct list_s{
    link head;
    link tail;
    int N;
} *List;

// prototipi funzioni statiche
static link LIST_newNode(Titolo val, link prev, link next);
static link find(List l, KeyT k);
static void delItem(List l, link x);
static link findSort(List l, KeyT k);


// private methods
static link LIST_newNode(Titolo val, link prev, link next){
    link x = malloc(sizeof *x);
    if(x == NULL) exit(-1);
    x->val = val;
    x->next = next;
    x->prev = prev;
    return x;
}

static link find(List l, KeyT k){
    if(l->N == 0) return NULL;
    link x;
    for(x=l->head; x!=NULL; x=x->next)
        if(KEYT_cmp(KEYT_get(x->val), k) == 0)
            return x;
    return NULL;
}

static void delItem(List l, link x){
    if(x == NULL) return;
    if(x->prev == NULL){
        LIST_delHead(l);
        return;
    }
    if(x->next == NULL){
        LIST_delTail(l);
        return;
    }
    else{
        x->prev->next = x->next;
        x->next->prev = x->prev;
        free(x);
        l->N--;
        return;
    }
}

static link findSort(List l, KeyT k){ // restituisce l'elemento uguale o successivo
    if(l->N == 0) return NULL;
    link x;
    for(x=l->head; x!=NULL; x=x->next)
        if(KEYT_cmp(KEYT_get(x->val), k) >= 0)
            return x;
        else if(KEYT_cmp(KEYT_get(x->val), k) > 0)
            return NULL;
    return NULL;
}


// public methods
List LIST_init(void){
    List l = malloc(sizeof *l);
    if(l == NULL) exit(-1);
    l->head = NULL;
    l->tail = NULL;
    l->N = 0;
    return l;
}

void LIST_insHead(List l, Titolo val){
    l->head = LIST_newNode(val, NULL, l->head);
    if(l->N == 0) // lista vuota, tail = head
        l->tail = l->head;
    l->N++;
}

void LIST_insTail(List l, Titolo val){
    switch(l->N){
        case 0:
            LIST_insHead(l, val);
            return;
        case 1: // l->tail ha il prev a null perché è il primo elemento
            l->tail = LIST_newNode(val, l->head, NULL);
            l->head->next = l->tail;
            break;
        default:
            l->tail = LIST_newNode(val, l->tail, NULL);
            l->tail->prev->next = l->tail;
            break;
    }
    l->N++;
}

Titolo LIST_search(List l, KeyT k){
    link x = find(l, k);
    if(x == NULL) return TITOLO_setVoid();
    return x->val;
}

void LIST_delHead(List l){
    switch(l->N){
        case 0:
            return;
        case 1:
            free(l->head);
            l->head = NULL;
            l->tail = NULL;
            l->N = 0;
            return;
        default:;    // >= 2
            link t = l->head;
            l->head = l->head->next;
            l->head->prev = NULL;
            free(t);
            l->N--;
            return;
    }
}

void LIST_delTail(List l){
    switch(l->N){
        case 0:
            return;
        case 1:
            LIST_delHead(l);
            return;
        default:;
            link t = l->tail;
            l->tail = l->tail->prev;
            l->tail->next = NULL;
            free(t);
            l->N--;
            return;
    }
}

Titolo LIST_extrHead(List l){
    Titolo val = l->head->val;
    LIST_delHead(l);
    return val;
}

void LIST_delKey(List l, KeyT k){
    link x = find(l, k);
    delItem(l, x);
}

Titolo LIST_extrKey(List l, KeyT k){
    link x = find(l, k);
    Titolo val = (x!=NULL)? x->val : TITOLO_setVoid();
    delItem(l, x);
    return val;
}

void LIST_sortIns(List l, Titolo item){
    link x = findSort(l, KEYT_get(item));
    if(x == NULL) LIST_insTail(l, item);
    else{
        link t = LIST_newNode(item, x->prev, x);
        if(x->prev == NULL) l->head = t;
        else x->prev->next = t;
        x->prev = t;
        l->N++;
    }
}

Titolo LIST_sortSearch(List l, KeyT k){
    link x = findSort(l, k);
    if(x == NULL || KEYT_cmp(KEYT_get(x->val), k) != 0) return TITOLO_setVoid();
    return x->val;
}

int LIST_sortFind(List l, KeyT k){
    link x = findSort(l, k);
    if(x == NULL || KEYT_cmp(KEYT_get(x->val), k) != 0) return 0;
    return 1;
}

void LIST_sortDel(List l, KeyT k){
    link x = findSort(l, k);
    if(x == NULL || KEYT_cmp(KEYT_get(x->val), k) != 0) return;
    delItem(l, x);
}

Titolo LIST_sortExtr(List l, KeyT k){
    link x = findSort(l, k);
    if(x == NULL || KEYT_cmp(KEYT_get(x->val), k) != 0) return TITOLO_setVoid();
    Titolo val = x->val;
    delItem(l, x);
    return val;
}

void LIST_sortUpdate(List l, Titolo item){
    link x = findSort(l, KEYT_get(item));
    if(x == NULL || KEYT_cmp(KEYT_get(x->val), KEYT_get(item)) != 0) return;
    x->val = item;
}

void LIST_show(List l){
    link x;
    for(x=l->head; x!=NULL; x=x->next)
        TITOLO_show(x->val);
}

void LIST_free(List l){
    link x, t;
    for(x=l->head; x!=NULL; x=t){
        t = x->next;
        TITOLO_free(x->val);
        free(x);
    }
    free(l);
}