#include<stdio.h>
#include<stdlib.h>
#include "ST.h"
#include "item.h"


struct symboltable{
    Item *a;
    int N;
    int M;
};

static int setSizeSet(int maxN, float alpha);
static int isFull(ST st, int i);



int hash(char *v, int M){
    int h = 0, base = 127;
    for(; *v != '\0'; v++);
        h = (base*h + *v) % M;
    return h;
}

int hashU(char *v, int M){
    int h, a = 31415, b = 27183;
    for(h = 0; *v != '\0'; v++, a = a*b % (M-1))
        h = (a*h + *v) % M;
    return h;
}

int hash2(char *k, int M){
    int h = 0, base = 127;
    for( ; *k != '\0'; k++)
        h = (base*h + *k) % M;
    h = ((h%97) + 1) % M;
    if(h == 0) h = 1;
    return h;
}




ST ST_init(int maxN, float alpha){
    int i;
    ST st = malloc(sizeof(*st));
    st->N = 0;
    st->M = setSizeSet(maxN, alpha);
    if(st->M == -1) st = NULL;
    else{
        st->a = malloc(st->M * sizeof(Item));
        for(i = 0; i < st->M; i++)
            st->a[i] = ITEM_setNull();
    }
    return st;
}

static int isFull(ST st, int i){
    if(ITEM_checkNull(st->a[i])) return 0;
    return 1;
}

int ST_sizeSet(ST st){
    return st->M;
}

static int setSizeSet(int maxN, float alpha){
    int primes[29] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109};
    int i = 0;

    if(maxN < primes[28] * alpha){
        for(i = 0; i < 29; i++)
            if(maxN <= primes[i] * alpha)
                return primes[i];
    }
    printf("Too many entries\n");
    return -1;
}

int ST_insert(ST st, Item item){
    int i = hash(KEY_get(&item), st->M);
    int j = hash2(KEY_get(&item), st->M);
    while(isFull(st, i))
        i = (i + j) % st->M;
    st->a[i] = item;
    st->a[i].value = i;
    st->N++;
    return i;
}

Item ST_searchItem(ST st, Key key){
    int i = hash((char*) key, st->M);
    int j = hash2((char*) key, st->M);
    while(isFull(st, i))
        if(KEY_cmp(key, KEY_get(&st->a[i])) == 0)
            return st->a[i];
        else i = (i+j)%st->M;
    return ITEM_setNull();
}

int ST_searchId(ST st, Key key){
    int i = hash((char*) key, st->M);
    int j = hash2((char*) key, st->M);
    while(isFull(st, i))
        if(KEY_cmp(key, KEY_get(&st->a[i])) == 0)
            return i;
        else i = (i+j)%st->M;
    return -1;
}

void ST_delete(ST st, Key key){
    int i = hash((char*) key, st->M);
    int j = hash2((char*) key, st->M);
    Item tmp;

    while(isFull(st, i))
        if(KEY_cmp(key, KEY_get(&st->a[i])) == 0) break;
        else i = (i+j)%st->M;
    if(ITEM_checkNull(st->a[i])) return;
    st->a[i] = ITEM_setNull();
    st->N--;
    i = (i+j)%st->M;
    while(isFull(st, i)){
        tmp = st->a[i];
        st->a[i] = ITEM_setNull();
        st->N--;
        ST_insert(st, tmp);
        i = (i+j)%st->M;
    }
}

void ST_free(ST st){
    free(st->a);
    free(st);
}

int ST_count(ST st){
    return st->N;
}

Item* ST_getArray(ST st){
    Item* tmp = malloc(st->N * sizeof(Item));
    int i, j = 0;
    for(i = 0; i < st->M; i++)
        if(!ITEM_checkNull(st->a[i]))
            tmp[j++] = st->a[i];
    return tmp;
}

Item ST_searchByIndex(ST st, int index){
    return st->a[index];
}