#include "equipArray.h"

/* ADT di prima classe collezione di oggetti di equipaggiamento */
// typedef struct equipArray_s *equipArray_t; 
struct equipArray_s {
    int equip[EQUIP_SLOT];
    int index;
};

/* creatore e disruttore */
equipArray_t equipArray_init(){
    equipArray_t equipArray = malloc(sizeof(struct equipArray_s));
    for(int i=0; i < EQUIP_SLOT; i++)
        equipArray->equip[i] = -1;
    equipArray->index = 0;
    return equipArray;
}
void equipArray_free(equipArray_t equipArray){
    free(equipArray);
}

/* quanti equipaggiamenti sono in uso */
int equipArray_inUse(equipArray_t equipArray){
    return equipArray->index;
}

/* scrittura su file */
void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray, char* prefix){
    for(int i=0; i < equipArray->index; i++){
        fprintf(fp, "%s", prefix);
        invArray_printByIndex(fp, invArray, equipArray->equip[i]);
        printf("\n");
    }
}

/* modifica equipaggiamento scegliendo un oggetto da inventario */
void equipArray_update(equipArray_t equipArray, invArray_t invArray){
    if(equipArray->index == EQUIP_SLOT-1){
        printf("Equipaggiamento pieno, non puoi aggiungere altri oggetti\n");
        return;
    }

    char name[LEN];
    int index= -1, i;
    printf("Inserisci il nome dell'oggetto da modificare: ");
    scanf("%s", name);

    index = invArray_searchByName(invArray, name); // se esiste l'oggetto
    if(index == -1){ // oggetto non esistente
        printf("Oggetto non trovato.\n");
        return;
    } else {
        for(int i = 0; i <equipArray_inUse(equipArray); i++)
            if(equipArray->equip[i] == index){ // oggetto già equipaggiato
                equipArray_remove(equipArray, i);
                return;
            }
        equipArray_insert(equipArray, invArray, index);
        return;
    }
}

/* torna indice (nel vettore inventario) dell'oggetto in posizione index (0..EQUIP_SLOT-1) di equipArray */
int equipArray_getEquipByIndex(equipArray_t equipArray, int index){
    if(index < 0 || index > EQUIP_SLOT-1){
        printf("Indice non valido\n");
        return -1;
    }
    return equipArray->equip[index];
}

/* Inserisce un singolo equipaggiamento nell'inventario */
void equipArray_insert(equipArray_t equipArray, invArray_t invArray, int index){
    if(equipArray->index == EQUIP_SLOT-1){
        printf("Equipaggiamento pieno, non puoi aggiungere altri oggetti\n");
        return;
    }

    equipArray->equip[equipArray->index] = index;
    equipArray->index++;
}

/* Rimuove oggetto a posizione index */
void equipArray_remove(equipArray_t equipArray, int index){
    int i=0;
    for(i=index; i<equipArray->index-1; i++)
        equipArray->equip[i] = equipArray->equip[i+1];
    
    equipArray->equip[i] = -1;
    equipArray->index--;
}