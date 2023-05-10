#include "pgList.h"

/* ADT di prima classe collezione di personaggi */
// typedef struct pgList_s* pgList_t;
struct pgList_s { // struct wrapper
    struct pgList_n* head;
    struct pgList_n* tail;
};

struct pgList_n{ // nodo della lista
    pg_t pg;
    struct pgList_n* next;
};

/* creatore e distruttore */
pgList_t pgList_init(){
    pgList_t pgList = (struct pgList_s*) malloc(sizeof(struct pgList_s));
    pgList->head = (struct pgList_n*) NULL;
    pgList->tail = (struct pgList_n*) NULL;
    return pgList;
}

void pgList_free(pgList_t pgList){
    struct pgList_n* tmp = pgList->head;
    while(tmp != NULL){
        pgList->head = tmp->next;
        pg_clean(&tmp->pg);
        free(tmp);
        tmp = pgList->head;
    }
    free(pgList);
}

/* nuovo nodo */
struct pgList_n* newNode(){
    struct pgList_n* newNode = (struct pgList_n*) malloc(sizeof(struct pgList_n));
    newNode->next = NULL;
    return newNode;
}

/* lettura e scrittura su file */
void pgList_read(FILE *fp, pgList_t pgList){
    struct pgList_n* this = pgList->head;
    struct pgList_n* prev;
    pg_t pg;

    while(pg_read(fp, &pg) == 1){
        pgList_insert(pgList, pg);
        // printf("%s %s\n", pgList->tail->pg.cod, pgList->tail->pg.nome);
    }
}

void pgList_print(FILE *fp, pgList_t pgList, invArray_t invArray){
    struct pgList_n* this = pgList->head;

    for(; this != NULL; this = this->next){
        pg_print(fp, &this->pg, invArray);
    }

}

/* inserimento di un nuovo personaggio */
void pgList_insert(pgList_t pgList, pg_t pg){
    if(pgList->head == NULL) // lista nulla
        pgList->head = pgList->tail = newNode();
    else{
        pgList->tail->next = newNode();
        pgList->tail = pgList->tail->next;
    }
    pgList->tail->pg = pg;
}

/* cancellazione con rimozione */
void pgList_remove(pgList_t pgList, char* cod){
    struct pgList_n* this = pgList->head;

    // prima occorrenza
    if(strcmp(pgList->head->pg.cod, cod) == 0){
        pgList->head = this->next;
        pg_clean(&this->pg);
        free(this);
        return;
    }

    for(; this != NULL; this = this->next){
        if(strcmp(this->pg.cod, cod) == 0){
            pg_clean(&this->pg);
            free(this);
            return;
        }
    }
}

/* ricerca per codice, ritornando il puntatore */
pg_t *pgList_searchByCode(pgList_t pgList, char* cod){
    struct pgList_n* this = pgList->head;

    for(; this != NULL; this = this->next){
        if(strcmp(this->pg.cod, cod) == 0)
            return &this->pg;
    }
    return NULL;
}
