#ifndef __TITOLO_H
#define __TITOLO_H

#include "BST.h"

#define LEN (20+1)

typedef struct titolo_s* Titolo;
typedef char* KeyT;
typedef int Name;

Titolo TITOLO_init();
Titolo TITOLO_setCode(Titolo A, char *code);
BST TITOLO_getQt(Titolo A);
int TITOLO_eq(Titolo A, Titolo B);
int TITOLO_neq(Titolo A, Titolo B);
int TITOLO_cmp(Titolo A, Titolo B);
void TITOLO_show(Titolo A);
Titolo TITOLO_setVoid();
Titolo TITOLO_setNull();
void TITOLO_free(Titolo A);

KeyT KEYT_get (Titolo k);
int KEYT_cmp (KeyT k1, KeyT k2);


#endif