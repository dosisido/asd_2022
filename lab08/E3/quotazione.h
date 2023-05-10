#ifndef __QUOTAZIONE_H
#define __QUOTAZIONE_H
#include<stdio.h>
#include "datetime.h"

typedef struct quatazione_giornaliera{
    Datetime dt;
    unsigned int numerator;
    unsigned int denominator;
    unsigned short int null;
} Quotazione;
typedef Datetime KeyQ;
typedef int Name;

int QUOTAZIONE_eq(Quotazione A, Quotazione B);
int QUOTAZIONE_neq(Quotazione A, Quotazione B);
int QUOTAZIONE_isNull(Quotazione A);
void QUOTAZIONE_show(Quotazione A);
Quotazione QUOTAZIONE_setDate(Quotazione A, Datetime dt);
Quotazione QUOTAZIONE_setVoid();
Quotazione QUOTAZIONE_setNull();
Quotazione QUOTAZIONE_setND(Quotazione A, int n, int d);
Quotazione QUOTAZIONE_addND(Quotazione A, int n, int d);
float QUOTAZIONE_value(Quotazione A);

KeyQ KEYQ_get (Quotazione k);
int KEYQ_cmp (KeyQ k1, KeyQ k2);
KeyQ KEYQ_scan(FILE *fp);




#endif