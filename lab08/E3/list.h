#ifndef __LIST_H
#define __LIST_H

#include "titolo.h"

typedef struct node_s node, *link;

typedef struct list_s *List;

List LIST_init(void);
void LIST_insHead(List l, Titolo val);
void LIST_insTail(List l, Titolo val);
Titolo LIST_search(List l, KeyT k);
void LIST_delHead(List l);
void LIST_delTail(List l);
Titolo LIST_extrHead(List l);
void LIST_delKey(List l, KeyT k);
Titolo LIST_extrKey(List l, KeyT k);
void LIST_sortIns(List l, Titolo item);
Titolo LIST_sortSearch(List l, KeyT k);
int LIST_sortFind(List l, KeyT k);
void LIST_sortDel(List l, KeyT k);
Titolo LIST_sortExtr(List l, KeyT k);
void LIST_sortUpdate(List l, Titolo item);
void LIST_show(List l);
void LIST_free(List l);





#endif