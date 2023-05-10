#ifndef __ST_H
#define __ST_H

#include "item.h"

typedef struct symboltable *ST;

ST ST_init(int MaxN, float r);
int ST_sizeSet(ST st);
int ST_insert(ST st, Item Val);
Item ST_searchItem(ST st, Key k);
int ST_searchId(ST st, Key key);
void ST_delete(ST st, Key k);
void ST_display(ST st);
void ST_free(ST st);
int ST_count(ST st);
int ST_empty(ST st);
Item* ST_getArray(ST st);
Item ST_getOneItem(ST st);
Item ST_searchByIndex(ST st, int index);





# endif