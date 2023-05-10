/*
 * Riccardo Rosin 284211
 * Lab 1 Es 1
*/
#include<stdio.h>
#define SRC "beLla lavanderina che lava i fazzoletti per i poverelli della citta"
#define REGEXP "\\ae\\Al"


typedef enum casi{punto, quadre, negato, minuscolo, maiuscolo, lettera} casi;
typedef enum bool{False, True} bool;

bool TRUNCATE = False; // decide se stampare tutta la stringa o solo la corrispondenza

casi selezionaRegexp(char* thisReg){
    // svitch per la selezione dell'operazione da eseguire
    switch( *thisReg ){
        case '.':
            return punto;
            break;
        case '[':
            thisReg++;
            if(*thisReg == '^'){
                return negato;
            }
            return quadre;
            break;
        case '\\':
            thisReg++;
            return *thisReg == 'a' ? minuscolo : maiuscolo;
            break;
    }
    return lettera;
}

bool findMultiCar(char car, char **regexp){
    // controllo degi elementi tra le parentesi della regexp
    char* thisReg = *regexp;
    while(**regexp != ']') (*regexp)++; (*regexp)++;
    while(*thisReg != ']'){
        if(car == *thisReg) return True;
        thisReg++;
    }
    return False;
}

bool isMatch(char car, char **thisReg){
    // ritorna se è match o no, incrementa il puntatore alla regexp al nuovo elemento
    switch(selezionaRegexp(*thisReg)){
        case punto:
            (*thisReg)++;
            return True;
            break;
        case quadre:
            (*thisReg)++;
            // funzione per vedere se c'è la corrispondenza
            if(findMultiCar(car, thisReg) == True)
                return True;
            break;
        case negato:
            (*thisReg) +=2;
            if(findMultiCar(car, thisReg) != True)
                return True;
            break;
        case minuscolo:
            (*thisReg) +=2;
            if(car >= 'a' && car <= 'z')
                return True;
            break;
        case maiuscolo:
            (*thisReg) +=2;
            if(car >= 'A' && car <= 'Z')
                return True;
            break;
        case lettera:
            if(car == *((*thisReg)++))
                return True;
            break;
    }
    return False;
}

char *cercaRegexp(char *src, char *regexp){
    // cerca iterativamente la regexp
    bool found;
    char *thisCar = src,
         *flag = NULL,
         *thisReg = regexp;

    while(*thisCar != '\0'){
        found = isMatch(*thisCar, &thisReg);
        
        if(found == True && *thisReg == '\0'){ // se sono alla fine della regexp
            if(TRUNCATE == True)
                *(thisCar + 1) = '\0';
            return flag;
        }

        if(found == False){ // se non ho corrispondenza rinizio a cercare
            thisReg = regexp;
            if(flag != NULL){
                thisCar = flag;
                flag = NULL;
            }
        }else 

        if(flag == NULL) // se ho trovato la prima corrispondenza della regex
            flag = thisCar;


        thisCar++;
    }


    return NULL;
}

/* 
    parto dalla stinga intera, ad ogni iterazione riduco la stringa, se trovo corrispondenza vado avanti, altrimenti rimando indietro
    termino finita una delle due stringhe
*/
bool rico(char *src, char *regexp){
    // cerco ricorsivamente la regexp confrontando carattere per carattere
    if(*regexp == '\0'){if(TRUNCATE == True) *src = '\0'; return True;}
    // se trova corrispondenza, vado avanti con entrambe le stringhe 
    // regexp incrementato in isMatch
    return isMatch(*src, &regexp) == True ? rico(src+1, regexp) : False;
}

char *cercaRegexpRico(char *src, char *regexp){
    // chiama la funzione ricorsiva su ogni carattere del vettore
    while(*(++src) != '\0')
        if(rico(src-1, regexp) == True)
            return src-1;
    return False;
}

int main(int argc, char* argv){
    char* pointer;
    if(True){
        char str[] = SRC;       // faccio questo perché è implementato il  metodo per restituire solo la porzione selezionata dalla regex modificando la variabile globale
        pointer = cercaRegexp(str, REGEXP);
        printf("%s\n", pointer);
    }
    if(True){
        char str[] = SRC;
        pointer = cercaRegexpRico(str, REGEXP);
        printf("%s\n", pointer);
    }

    return 0;
}
