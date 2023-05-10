#include<stdio.h>
#include<stdlib.h>
#include <time.h>

#define STRLEN (100+1)
#define FILE_IN "elementi.txt"
#define N_DIAGONALS 3                               // quante diagonali presentare
#define N_ELEMENTS 5                                // 5 elemnti per diagonale
#define INIT_DIAGONAL_ROTATION 1                    // inizia frontalmente
#define DIFFICULTY_FOR_MULTIPLIER 8                 // difficoltà che deve avere l'ultimo elemento per avere il bonus
#define DBG 0

typedef short unsigned int this_int;

typedef struct {
    char name[STRLEN];
    this_int type;                           // enum {transizione, acrobatico indietro, acrobatico avanti}
    this_int entry;                          // enum {spalle, frontalmente}
    this_int exit;                           // enum {spalle, frontalmente}
    this_int start;                          // enum {primo, serve qualcosa prima}
    this_int end;                            // enum {ultimo, serve qualcosa dopo}
    float value;
    this_int difficulty;
} element;
typedef struct {
    element* elements;
    this_int size;
} elements_t;

typedef struct{
    this_int sequence[N_ELEMENTS];
    float value;
    this_int difficulty;
    this_int n;
} diagonal;
typedef struct{
    diagonal* diagonal;
    this_int size;
    this_int maxSize;
} diagonals_t;

typedef struct{
    this_int diagonal[N_DIAGONALS];
    float value;
    this_int difficulty;
} program_t;

typedef enum {False, True} bool;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////// costruttori e distruttori
void init(elements_t* data){
    int i;
    FILE *fin;
    if ((fin = fopen(FILE_IN, "r")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", FILE_IN);
        exit(-1);
    }

    fscanf(fin, " %d ", &data->size);
    data->elements = (element*) malloc(sizeof(element) * data->size);

    for(i=0; i < data->size; i++)
        fscanf(fin, " %s %d %d %d %d %d %f %d ",
            data->elements[i].name,
            &data->elements[i].type,
            &data->elements[i].entry,
            &data->elements[i].exit,
            &data->elements[i].start,
            &data->elements[i].end,
            &data->elements[i].value,
            &data->elements[i].difficulty);

    fclose(fin);
}

void release(elements_t data){
    free(data.elements);
}

void releaseDiagonals(diagonals_t diagonals){
    free(diagonals.diagonal);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////// general
void printDiagonal(FILE* fout, const diagonal d, const elements_t data, bool printMultiplier){
    for(int i=0; i < d.n; i++)
        fprintf(fout, "%s  ", data.elements[d.sequence[i]].name);

    if(printMultiplier && data.elements[d.sequence[d.n-1]].difficulty >= DIFFICULTY_FOR_MULTIPLIER)
        fprintf(fout, "Punteggio: %2.3f x1.5  Difficolta': %3d\n", d.value, d.difficulty);
    else fprintf(fout, "Punteggio: %2.3f  Difficolta': %3d\n", d.value, d.difficulty);
}

void printProgram(const elements_t data, const diagonals_t diagonals, const program_t program){
    #if DBG
    float tmp = 0.0;
    printf("\n");
    #endif

    printf("Programma con punteggio %2.3f e difficolta' %d\n", program.value, program.difficulty);
    for(int i=0; i < N_DIAGONALS; i++){
        #if DBG
        printf("%4d: \t", program.diagonal[i]+1);
        tmp += diagonals.diagonal[program.diagonal[i]].value;
        #endif
        printDiagonal(stdout, diagonals.diagonal[program.diagonal[i]], data, (i == N_DIAGONALS-1) ? True : False);
    }
    #if DBG
    if(data.elements[diagonals.diagonal[program.diagonal[N_DIAGONALS-1]].sequence[diagonals.diagonal[program.diagonal[N_DIAGONALS-1]].n-1]].difficulty >= DIFFICULTY_FOR_MULTIPLIER)
        tmp += diagonals.diagonal[program.diagonal[N_DIAGONALS-1]].value*0.5;
    printf("Ricalcolo valore: %2.3f\n", tmp);
    #endif
}

float calculateValue(const elements_t data, const diagonals_t diagonals, const program_t program){
    float tmp = 0.0;

    for(int i=0; i < N_DIAGONALS && program.diagonal[i] != -1; i++)
        tmp += diagonals.diagonal[program.diagonal[i]].value;
    
    if(data.elements[diagonals.diagonal[program.diagonal[N_DIAGONALS-1]].sequence[diagonals.diagonal[program.diagonal[N_DIAGONALS-1]].n-1]].difficulty >= DIFFICULTY_FOR_MULTIPLIER)
        tmp += diagonals.diagonal[program.diagonal[N_DIAGONALS-1]].value*0.5;
    return tmp;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////// calcolo della diagonale
bool check(const element e, const int entryRotation, const int difficulty, const int DD, const int pos){
    if(pos == 0)
        if(e.start == 1)        // provo a metterlo come primo ma serve qualcosa prima
            return False;

    if(e.difficulty + difficulty > DD)
        return False;
    if(e.entry != entryRotation)
        return False;
    return True;
}

bool checkDiagonal(const elements_t data, const int buff[], const int pos){
    int i;


    for(i=0; i < pos-1; i++){
        if(data.elements[buff[i]].end == 1)         // se trovo elementi che non vogliono essere seguiti da altri
            return False;
    }
    if(data.elements[buff[0]].start == 1)           // se il primo non vuole nulla prima
        return False;

    // includere almeno un elemento acrobatico
    for(i=0; i < pos; i++){
        if(data.elements[buff[i]].type == 1 || data.elements[buff[i]].type == 2)
            return True;
    }
    return False;
}

void addDiagonal(diagonals_t* diagonals, const int* buff,const int pos, const float score, const int difficulty){
    if(diagonals->size == diagonals->maxSize){
        if(diagonals->maxSize == 0){
            diagonals->maxSize = 1;
            diagonals->diagonal = (diagonal*) malloc(sizeof(diagonal) * diagonals->maxSize);
        }
        else{
            diagonals->maxSize *= 2;
            diagonals->diagonal = (diagonal*) realloc(diagonals->diagonal, sizeof(diagonal) * diagonals->maxSize);
        }
    }

    diagonals->diagonal[diagonals->size].value = score;
    diagonals->diagonal[diagonals->size].difficulty = difficulty;
    diagonals->diagonal[diagonals->size].n = pos;
    for(int i=0; i < N_ELEMENTS; i++)
        diagonals->diagonal[diagonals->size].sequence[i] = buff[i];
    diagonals->size++;
}

void calculateDiagonalsR(   const elements_t data, diagonals_t* diagonals, const int DD,
                            const int pos, int buff[], const int rotation, const float score, const int difficulty){

    // controllo di termine
    if(pos != 0){
        if(checkDiagonal(data, buff, pos)){
            addDiagonal(diagonals, buff, pos, score, difficulty);
        }
    }
    if(pos == N_ELEMENTS) return;

    for(int i=0; i < data.size; i++){
        // controllo di validità
        if(!check(data.elements[i], rotation, difficulty, DD, pos)) continue;

        // aggiunta elemento
        buff[pos] = i;

        // chiamata ricorsiva
        calculateDiagonalsR(
            data, diagonals, DD,
            pos+1, buff, data.elements[i].exit, score+data.elements[i].value, difficulty+data.elements[i].difficulty
        );

        // rimozione elemento
        #if DBG
        buff[pos] = -1;
        #endif
    }
}

void calculateDiagonals(const elements_t data, diagonals_t* diagonals, const int DD){
    int buff[N_ELEMENTS];
    calculateDiagonalsR(data, diagonals, DD, 0, buff, INIT_DIAGONAL_ROTATION, 0.0, 0);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////// calcolo programma
bool checkProgram(const elements_t data, const diagonals_t diagonals, program_t* best_program, const program_t* buff){
    int i, j;
    bool acrobaticoAvanti, acrobaticoIndietro; int sequenza;
    element* e;

    acrobaticoAvanti = acrobaticoIndietro = False;
    sequenza = False;              // elementi in sequenza: 0 = nessuno, 1 = ne ho trovato 1, 2 = ci sono entrambi
    for(i=0; i<N_DIAGONALS; i++){
        for(j=0; j<diagonals.diagonal[buff->diagonal[i]].n; j++){
            e = &data.elements[diagonals.diagonal[buff->diagonal[i]].sequence[j]];

            // un elemento acrobatico avanti ed uno indietro
            if(e->type == 1) acrobaticoAvanti = True;   // ne esiste almeno uno avanti
            if(e->type == 2) acrobaticoIndietro = True; // ne esiste almeno uno indietro

            // due elementi acrobatici in sequenza
            if(sequenza != 2)
                if(e->type == 1 || e->type == 2)
                    if(sequenza == 0) sequenza = 1;
                    else sequenza = 2;
                else sequenza = 0;
        }
        if(sequenza != 2) sequenza = 0;
    }
    if(!acrobaticoAvanti || !acrobaticoIndietro) return False;
    if(sequenza != 2) return False;

    // se sono qua vuol dire che l'elemento è valido
    // calcolo punteggio e vedo se è meglio del massimo

    // se punteggio elemento finale >= 8 -> punteggio ultima diagonale x1.5 volte
    float tmpValue = buff->value;
    if(data.elements[diagonals.diagonal[buff->diagonal[N_DIAGONALS-1]].sequence[diagonals.diagonal[buff->diagonal[N_DIAGONALS-1]].n-1]].difficulty >= DIFFICULTY_FOR_MULTIPLIER){
        // printProgram(data, diagonals, *buff);
        tmpValue += diagonals.diagonal[buff->diagonal[N_DIAGONALS-1]].value * .5;
    }
    if(best_program->value < tmpValue){
        *best_program = *buff;
        best_program->value = tmpValue;
    }

    return True;
}

void calculateProgramR(const elements_t data, const diagonals_t diagonals, const int DP, program_t* best_program, program_t* buff, const int pos, const float value){

    // controllo di termine
    if(pos == N_DIAGONALS){
        buff->value = value;
        checkProgram(data, diagonals, best_program, buff);
        return;
    }

    for(int i=0; i < diagonals.size; i++){
        // controllo di validità
        if(buff->difficulty + diagonals.diagonal[i].difficulty > DP) continue;

        // aggiunta elemento
        buff->diagonal[pos] = i;
        // buff->value += diagonals.diagonal[i].value;
        buff->difficulty += diagonals.diagonal[i].difficulty;

        // chiamata ricorsiva
        calculateProgramR(data, diagonals, DP, best_program, buff, pos+1, value + diagonals.diagonal[i].value);

        // rimozione elemento
        #if DBG
        buff->diagonal[pos] = -1;
        #endif
        // buff->value -= diagonals.diagonal[i].value;
        buff->difficulty -= diagonals.diagonal[i].difficulty;
    }
}

void calculateProgram(const elements_t data, const diagonals_t diagonals, const int DP, program_t* program){
    program_t buff;
    buff.value = 0.0;
    buff.difficulty = 0;
    calculateProgramR(data, diagonals, DP, program, &buff, 0, 0.0);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////// processamento dei dati
void process(const elements_t data, const int DD, const int DP){
    diagonals_t diagonals;
    diagonals.size = 0;
    diagonals.maxSize = 0;
    program_t program;
    program.value = 0;
    program.difficulty = 0;

    calculateDiagonals(data, &diagonals, DD);
    diagonals.diagonal = (diagonal*) realloc(diagonals.diagonal, sizeof(diagonal) * (diagonals.maxSize = diagonals.size)); // per provare a salvare spazio

    #if DBG
    FILE *fout;
    if ((fout = fopen("out.txt", "w+")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", "out.txt");
        exit(-1);
    }
    for(int i=0; i < diagonals.size; i++)
        printDiagonal(fout, diagonals.diagonal[i], data, False);
    fclose(fout);
    printf("Inizio con il calcolo: \n");
    #endif

    calculateProgram(data, diagonals, DP, &program);
    printProgram(data, diagonals, program);

    releaseDiagonals(diagonals);
}


int main(int argc, char* argv[]){
    int DD, DP;
    elements_t data;
    time_t start;

    init(&data);

    printf("(DD) Difficolta' massima di una diagonale: ");
    scanf(" %d", &DD);
    printf("(DP) Difficolta' massima del programma: ");
    scanf(" %d", &DP);

    printf("\n");
    start = clock();

    process(data, DD, DP);

    printf("\nTempo impiegato: %f\n", ((double) (clock() - start)) / CLOCKS_PER_SEC);

    release(data);
    return 0;
}