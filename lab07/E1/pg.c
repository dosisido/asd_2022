#include "pg.h"

/* quasi ADT personaggio; si noti che si tratta di un composto con riferimento
a un equipArray di proprieta' */
// typedef struct pg_s {
//     char cod[LEN];
//     char nome[LEN];
//     char classe[LEN];
//     stat_t b_stat, eq_stat;
//     equipArray_t equip;
// } pg_t;

/* lettura e scrittura su file */
int pg_read(FILE *fp, pg_t *pgp){
    if(fscanf(fp, " %s %s %s ", pgp->cod, pgp->nome, pgp->classe) != 3) return 0;
    stat_read(fp, &pgp->b_stat);
    pgp->eq_stat = pgp->b_stat;
    pgp->equip = equipArray_init();
    return 1;
}

/* non essendo struct dinamica, pulisce chiamando il distruttore di equipArray */
void pg_clean(pg_t *pgp){
    equipArray_free(pgp->equip);
}

void pg_print(FILE *fp, pg_t *pgp, invArray_t invArray){
    fprintf(fp, "%s %s %s ", pgp->cod, pgp->nome, pgp->classe);
    stat_print(fp, &pgp->eq_stat, MIN_STAT);
    printf("\nEquipaggiamento:\n");
    equipArray_print(fp, pgp->equip, invArray, "\t");
    printf("\n");
}

/* modifica personaggio aggiungendo/togliendo un equipaggiamento selezionato da inventario:
di fatto e' sufficiente chiamare l'opportuna funzione dal modulo equipArray */
void pg_updateEquip(pg_t *pgp, invArray_t invArray){
    equipArray_update(pgp->equip, invArray);
    // stat_setZero(&pgp->eq_stat);
    pgp->eq_stat = pgp->b_stat;
    int i=0, invIndex;
    while((invIndex = equipArray_getEquipByIndex( pgp->equip, i)) != -1){
        stat_sum(&pgp->eq_stat, inv_getStat(invArray_getByIndex(invArray, invIndex)));
        i++;
    }
}