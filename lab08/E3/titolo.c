#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "titolo.h"
#include "bst.h"

struct titolo_s{
    char codice[LEN];
    BST qt;
};

Titolo TITOLO_init(){
    Titolo A = malloc(sizeof(struct titolo_s));
    A->qt = BST_init();
    return A;
}

Titolo TITOLO_setCode(Titolo A, char *code){
    strcpy(A->codice, code);
    return A;
}

BST TITOLO_getQt(Titolo A){
    return A->qt;
}

int TITOLO_eq(Titolo A, Titolo B) {
    return (A == B);
}

int TITOLO_neq(Titolo A, Titolo B) {
    return (A != B);
}

int TITOLO_cmp(Titolo A, Titolo B) {
    return KEYT_cmp(KEYT_get(A), KEYT_get(B));
}

void TITOLO_show(Titolo A) {
    return ;
}

Titolo TITOLO_setVoid(){
    return (Titolo) NULL;
}

Titolo TITOLO_setNull(){
    return TITOLO_setVoid();
}

void TITOLO_free(Titolo A){
    BST_free(A->qt);
    free(A);
}


KeyT KEYT_get (Titolo A){
    return A->codice;
}

int KEYT_cmp (KeyT k1, KeyT k2){
    return strcmp(k1, k2);
}
