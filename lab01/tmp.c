#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 30
 
 
char *cercaRegexp(char *src, char *regexp);
int confronto(char sorgente, char *reg);
int scan(char una, char *due);
 
int main() {
    char sorgente[N] = "Portamoto", exp[N] = "\\aoto";
    char *p;
    p = cercaRegexp( sorgente, exp);
    if(p==NULL){
        printf("Ricorrenza non trovata!");
    }
    else{
        printf("%s", p);
    }
    return 0;
}
 
char *cercaRegexp(char *src, char *regexp){
    int valcfr;
    char *ins, *inr;
    ins = NULL;
    inr = regexp; //Salvo la posizione iniziale della regexp
    while(*src != '\0'){
        valcfr = confronto(*src,  regexp);
        if( valcfr > 0 ){ // ho trovato occorrenza
            regexp=regexp+valcfr; // incremento regex

            if(*regexp=='\0'){
                // esco se regex finita
                return ins;
            }

            if(ins == NULL){ // sono alla prima occorrenza
                ins = src;
            }
            src++;
            
        }
        else{
            //entro qui quando NON ho trovato l'occorrenza (cioè valcfr è 0)
            
            regexp = inr; // Reinizio da capo a controllare
            if(ins != NULL){
                src = ins; //riavvolgo la sorgente
                ins = NULL; //svuoto l'inizio sorgente
            }
            src++; //vado in avanti di uno nella sorgente
        }
 
    }
    return NULL;
}
 
int confronto(char sorgente, char *reg){
    int qnt, valscan=0;
    if(*reg == '.'){
        return 1;
    }
    if(*reg == '['){
        for(qnt=0; *(reg + qnt)!= ']'; qnt++);
        qnt++; //Conto anche la prima parentesi che il for esclude '['
        reg++;
        if(*reg == '^'){
            reg++;
            valscan = scan(sorgente, reg);
            if(valscan == 0){
                return qnt;
            }
            else{
                return 0;
            }
        }
        else{
            valscan = scan(sorgente, reg);
            printf("Sono nel caso quadre normali\n");
            if(valscan == 1){
                return qnt;
            }
            else{
                return 0;
            }
        }
    }
    if(*reg == '\\'){
        reg++;
        if(*reg == 'A'){
            if(sorgente >= 'A' && sorgente <= 'Z'){
                return 2;
            }
            else{ return 0; }
        }
        if(*reg == 'a'){
            if(sorgente >= 'a' && sorgente <= 'z'){
                return 2;
            }
            else{ return 0; }
        }
    }
    if(sorgente == *reg){
        return 1;
    }
    else{
        return 0;
    }
}
 
int scan(char una, char *due){
 
    while(*due != ']'){
        if(una==*due){
            return 1;
        }
        else{
            due++;
        }
    }
    return 0;
}