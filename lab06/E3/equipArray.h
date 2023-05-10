#ifndef EQUIPARRAY_H_DEFINED
#define EQUIPARRAY_H_DEFINED

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define EQUIP_SLOT 8

#include "invArray.h"

/* ADT di prima classe collezione di oggetti di equipaggiamento */
typedef struct equipArray_s *equipArray_t; 

/* creatore e disruttore */
equipArray_t equipArray_init();
void equipArray_free(equipArray_t equipArray);

/* quanti equipaggiamenti sono in uso */
int equipArray_inUse(equipArray_t equipArray);

/* scrittura su file */
void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray, char* prefix);

/* modifica equipaggiamento scegliendo un oggetto da inventario */
void equipArray_update(equipArray_t equipArray, invArray_t invArray);

/* torna indice (nel vettore inventario) dell'oggetto in posizione index (0..EQUIP_SLOT-1) di equipArray */
int equipArray_getEquipByIndex(equipArray_t equipArray, int index);

/* Si possono aggiungere altre funzioni se ritenute necessarie */

/* Inserisce un singolo equipaggiamento nell'inventario */
void equipArray_insert(equipArray_t equipArray, invArray_t invArray, int index);

/* Rimuove oggetto a posizione index */
void equipArray_remove(equipArray_t equipArray, int index);

#endif
