#include<stdio.h>
#include<stdlib.h>
#define FILE_TILES "tiles.txt"
#define FILE_BOARD "board.txt"

typedef struct {
    int id;
    int rotation;
} box;

typedef struct {
    char C1;
    int V1;
    char C2;
    int V2;
} tile;

typedef enum {false, true} bool;
typedef enum {orizzontale, verticale} direzione;

void init(box*** table, box*** best_table, tile** tiles, int** marck, int* r, int *c, int* t){
    int i, j;
    FILE* file_in;

    

    if((file_in = fopen(FILE_TILES, "r")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", FILE_TILES);
        exit(-1);
    }

    fscanf(file_in, " %d ", t);
    *tiles = (tile*) malloc(*t * sizeof(tile));
    *marck = (int*) calloc(*t,  sizeof(int));
    for(i=0; i<*t; i++)
        fscanf(file_in, " %c %d %c %d ",
            &(*tiles)[i].C1,
            &(*tiles)[i].V1,
            &(*tiles)[i].C2,
            &(*tiles)[i].V2
        );
    
    fclose(file_in);



    if((file_in = fopen(FILE_BOARD, "r")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", FILE_BOARD);
        exit(-1);
    }

    fscanf(file_in, " %d %d ", r, c);
    *table = (box**) malloc(*r * sizeof(box*));
    *best_table = (box**) malloc(*r * sizeof(box*));
    for(i=0; i<*r; i++){
        (*table)[i] = (box*) malloc(*c * sizeof(box));
        (*best_table)[i] = (box*) malloc(*c * sizeof(box));

        for(j=0; j<*c; j++){
            fscanf(file_in, " %d/%d", 
                &(*table)[i][j].id,
                &(*table)[i][j].rotation
            );
            (*marck)[(*table)[i][j].id] = 1;
        }
    }

    fclose(file_in);
}

void libera(box** table, box** best_table, tile* tiles, int* marck, int r){
    int i;
    for(i=0; i<r; i++){
        free(table[i]);
        free(best_table[i]);
    }
    free(table);
    free(best_table);
    free(tiles);
    free(marck);
}

void stampa_matrice(box** table, int r, int c){
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++)
            printf("%d/%d ", table[i][j].id, table[i][j].rotation);
        printf("\n");
    }
    printf("\n");
}

char getColor(int index, direzione dir, int rot, tile* tiles){
    switch(rot){
        case 0: // non ruotato
            if(dir == orizzontale)
                return tiles[index].C1;
            return tiles[index].C2;

        case 1:
            if(dir == orizzontale)
                return tiles[index].C2;
            return tiles[index].C1;
    }
}

int getValue(int index, direzione dir, int rot, tile* tiles){
    switch(rot){
        case 0: // non ruotato
            if(dir == orizzontale)
                return tiles[index].V1;
            return tiles[index].V2;
            
        case 1:
            if(dir == orizzontale)
                return tiles[index].V2;
            return tiles[index].V1;
    }
}

int calcolaPunteggio(const int R, const int C, box** table, tile* tiles){
    int i, j, value = 0, tmp;
    char colore;

    // leggo per righe
    for(i=0; i<R; i++){
        colore = getColor(table[i][0].id, orizzontale, table[i][0].rotation, tiles);
        tmp = getValue(table[i][0].id, orizzontale, table[i][0].rotation, tiles);
        
        for(j=1; j<C; j++)
            if(colore == getColor(table[i][j].id, orizzontale, table[i][j].rotation, tiles))
                tmp+= getValue(table[i][j].id, orizzontale, table[i][j].rotation, tiles);
            else{
                j = -1;
                break;
            }
        
        if(j!= -1) value+= tmp;        
    }

    // leggo per colonne
    for(j=0; j<C; j++){
        colore = getColor(table[0][j].id, verticale, table[0][j].rotation, tiles);
        tmp = getValue(table[0][j].id, verticale, table[0][j].rotation, tiles);
        
        for(i=1; i<R; i++)
            if(colore == getColor(table[i][j].id, verticale, table[i][j].rotation, tiles))
                tmp+= getValue(table[i][j].id, verticale, table[i][j].rotation, tiles);
            else{
                i = -1;
                break;
            }
        
        if(i!= -1) value+= tmp;        
    }
    
    return value;
}

void copyTable(const int R, const int C, box** table, box*** best_table){
    int i, j;
    for(i=0; i<R; i++)
        for(j=0; j<C; j++)
            (*best_table)[i][j] = table[i][j];
}

void calcola(int r, int c, const int R, const int C, const int T, box** table, tile* tiles, int* marck, int* maxValue, box*** best_table){
    int i;

    if(r == R){ // condizione di terminazione
        int value = calcolaPunteggio(R, C, table, tiles);
        if(value > *maxValue){
            *maxValue = value;
            copyTable(R, C, table, best_table);
        }
        return;
    }

    if(c == C){  // nuova riga
        calcola(r+1, 0, R, C, T, table, tiles, marck, maxValue, best_table);
        return;
    }

    if(table[r][c].id != -1){ // casella già occupata
        calcola(r, c+1, R, C, T, table, tiles, marck, maxValue, best_table);
        return;
    }

    for(i=0; i<T; i++){ // provo "tutte" le tessere
        if(marck[i] == 0){
            marck[i] = 1;   // tessera disponibile, la simulo in entrmbe le rotazioni
            
            table[r][c].id = i;
            table[r][c].rotation = 0;
            calcola(r, c+1, R, C, T, table, tiles, marck, maxValue, best_table);

            table[r][c].rotation = 1;
            calcola(r, c+1, R, C, T, table, tiles, marck, maxValue, best_table);

            marck[i] = 0;
            table[r][c].id = -1;
            table[r][c].rotation = -1;
        }

    }
}


int main(int argc, char** argv){

    int R, C, T, *marck, maxValue = 0;
    box** table, **best_table;
    tile* tiles;

    init(&table, &best_table, &tiles, &marck, &R, &C, &T);

    calcola(0, 0, R, C, T, table, tiles, marck, &maxValue, &best_table);

    printf("Massimo valore trovato e': %d\n", maxValue);
    stampa_matrice(best_table, R, C);

    libera(table, best_table, tiles, marck, R);
}