#include "inv.h"

/*

    // quasi ADT statistiche 
    typedef struct stat_s {
        int hp, mp, atk, def, mag, spr;
    } stat_t;

    // quasi ADT oggetto di inventario 
    typedef struct inv_s {
        char nome[LEN];
        char tipo[LEN];
        stat_t stat;
    } inv_t;

 */

// funzioni di input/output delle statistiche 
void stat_read(FILE *fp, stat_t *statp){
    fscanf(fp, "%d %d %d %d %d %d",
        &statp->hp,
        &statp->mp,
        &statp->atk,
        &statp->def,
        &statp->mag,
        &statp->spr
    );
    
}

void stat_print(FILE *fp, stat_t *statp, int soglia){
    fprintf(fp, "%5d %5d %5d %5d %5d %5d",
        (statp->hp) > soglia ? statp->hp : soglia,
        (statp->mp) > soglia ? statp->mp : soglia,
        (statp->atk) > soglia ? statp->atk : soglia,
        (statp->def) > soglia ? statp->def : soglia,
        (statp->mag) > soglia ? statp->mag : soglia,
        (statp->spr) > soglia ? statp->spr : soglia
    );
    
}

// funzioni di input/output di un oggetto dell'inventario 
void inv_read(FILE *fp, inv_t *invp){
    fscanf(fp, " %s %s ", invp->nome, invp->tipo);
    stat_read(fp, &invp->stat);
}

void inv_print(FILE *fp, inv_t *invp){
    fprintf(fp, "%20s %20s ", invp->nome, invp->tipo);
    stat_print(fp, &invp->stat, INT_MIN);
}

// ritorna il campo stat di un oggetto dell'inventario 
stat_t inv_getStat(inv_t *invp){
    return invp->stat;
}

/* somma di due satistiche */
void stat_sum(stat_t* s1, stat_t s2){
    s1->hp  += s2.hp ;
    s1->mp  += s2.mp ;
    s1->atk += s2.atk;
    s1->def += s2.def;
    s1->mag += s2.mag;
    s1->spr += s2.spr;
}

/* setta a 0 le statistiche */
void stat_setZero(stat_t *s1){
    s1->hp  = 0;
    s1->mp  = 0;
    s1->atk = 0;
    s1->def = 0;
    s1->mag = 0;
    s1->spr = 0;
}