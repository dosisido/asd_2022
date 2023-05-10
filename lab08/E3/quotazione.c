#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "quotazione.h"
#include "datetime.h"


int QUOTAZIONE_eq(Quotazione A, Quotazione B) {
    return (DT_cmp(A.dt, B.dt) == 0 && A.numerator == B.numerator && A.denominator == B.denominator && A.null == B.null);
}

int QUOTAZIONE_neq(Quotazione A, Quotazione B) {
    return !QUOTAZIONE_eq(A, B);
}

int QUOTAZIONE_isNull(Quotazione A) {
    return (A.null == 1);
}

float QUOTAZIONE_value(Quotazione A) {
    return (float) A.numerator / (float) A.denominator;
}

void QUOTAZIONE_show(Quotazione A) {
    printf("Valore della quotazione: %5.5f\n", QUOTAZIONE_value(A));
}

Quotazione QUOTAZIONE_setVoid(){
    Quotazione qt;
    qt.dt = DT_setNull();
    qt.numerator = 0;
    qt.denominator = 0;
    qt.null = 1;
    return qt;
}

Quotazione QUOTAZIONE_setNull(){
    return QUOTAZIONE_setVoid();
}

Quotazione QUOTAZIONE_setDate(Quotazione A, Datetime dt){
    A.dt = dt;
    A.null = 0;
    return A;
}

Quotazione QUOTAZIONE_setND(Quotazione A, int n, int d){
    A.numerator = n;
    A.denominator = d;
    A.null = 0;
    return A;
}

Quotazione QUOTAZIONE_addND(Quotazione A, int n, int d){
    A.numerator+= n;
    A.denominator+= d;
    A.null = 0;
    return A;
}

KeyQ KEYQ_get (Quotazione A){
    return A.dt;
}

int KEYQ_cmp (KeyQ k1, KeyQ k2){
    return DT_cmp((Datetime) k1, (Datetime) k2);
}

KeyQ KEYQ_scan(FILE *fp){
    Datetime key;
    key = DT_newDateFile(fp);           
    return key;
}