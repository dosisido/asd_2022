/*
 * Riccardo Rosin 284211
 * Lab 2 Es 3
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define fileIn "corse.txt"
#define fileOut "out.txt"
#define maxWord 30
#define numOrdinamento (1+4)


typedef struct dati{
    char id[maxWord+1];
    char start[maxWord+1];
    char end[maxWord+1];
    char date[maxWord+1];
    char hourI[maxWord+1];
    char hourF[maxWord+1];
    int delay;
} dati;

typedef enum comandi{stampa, o_data, o_tratta, o_partenza, o_arrivo, r_stazione, r_lettura, r_fine} comandi;
typedef enum order{none, data, tratta, partenza, arrivo} order;
typedef enum bool{False, True} bool;

int stampaRow = True; // stampa il recordo in riga o a blocco


bool insertLog(char fileTxt[], dati** log, int *dim){
    // with file, alloca e salva i dati
    int n, i;
    FILE *fin;
    if ((fin = fopen(fileTxt, "r")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", fileTxt);
        return False;
    }
    
    if(fscanf(fin, " %d ", dim)!=1) return True; // legge dimensione ed alloca dinamicamente vettore originale
    if((*log = (dati *) malloc(*dim * sizeof(dati))) == NULL) return False;
    

    for(i=0; i<*dim && !feof(fin); i++)
        fscanf(fin, "%s %s %s %s %s %s %d ", 
            (*log)[i].id, 
            (*log)[i].start, 
            (*log)[i].end, 
            (*log)[i].date, 
            (*log)[i].hourI, 
            (*log)[i].hourF, 
            &(*log)[i].delay
        );
    // log punta al puntatore del vettore di struct
    fclose(fin);
    return True;
}

bool startStrutture(char fileTxt[], dati** log, int *dim, dati**** ordinamento){
    // alloca vettori di puntaotri che mantengono gli ordinamenti
    int i;
    if(!insertLog(fileTxt, log, dim)) return False;
    
    *ordinamento = (dati***) malloc(numOrdinamento * sizeof(dati** ));
    if(*ordinamento == NULL) return False;


    for(i=0; i<numOrdinamento; i++){
        (*ordinamento)[i] = (dati**) malloc(*dim * sizeof(dati*));
    }
    
    // prima riga inizallizzata all'ordinamento di input
    for(i=0; i<*dim; i++)
        (*ordinamento)[0][i] = &(*log)[i];

    // inizializza NULL primo elemento di ogni riga a partire dalla seconda [1]
    for(i=1; i<numOrdinamento; i++)
        (*ordinamento)[i][0] = NULL;

}

void resetMem(dati** log, int *dim, dati ****ordinamento){
    free(*log);
    for(int i=0; i<numOrdinamento; i++){
        free((*ordinamento)[i]);
    }
    free(*ordinamento);
    *dim = 0;
}

bool resetAndStart(char fileTxt[], dati** log, int *dim, dati ****ordinamento){
    resetMem(log, dim, ordinamento);
    if(startStrutture(fileTxt, log, dim, ordinamento) ) return True;
    return False;
}

void copyArray(dati* log[], int lenLog, dati* destination[]){
    for(int i=0; i<lenLog; i++)
        destination[i] = log[i];
}

void print_log_row(dati *log, FILE *file){
    if(stampaRow){ fprintf(file, "%s %s %s %s %s %s %d\n", log->id, log->start, log->end, log->date, log->hourI, log->hourF, log->delay); return; }
    fprintf(file, "\tid: %s\n", log->id);
    fprintf(file, "\tinizio: %s\n", log->start);
    fprintf(file, "\tfine: %s\n", log->end);
    fprintf(file, "\tdata: %s\n", log->date);
    fprintf(file, "\tora inizio: %s\n", log->hourI);
    fprintf(file, "\tora fine: %s\n", log->hourF);
    fprintf(file, "\tritardo: %d\n\n", log->delay);
}

void print_log(dati *log[], int n, FILE *file){
    for(int i=0; i<n; i++)
        print_log_row(log[i], file);
}

int date_cmp(dati a, dati b){
    /*   
     *   1 se 1 > 2
     *   0 se 1 = 2
     *  -1 se 1 < 2
     */
    int n1, n2;
    // data
    sscanf(a.date, "%d", &n1);
    sscanf(b.date, "%d", &n2);
    if(n1>n2) return 1;
    else if(n2>n1) return -1;
    sscanf(a.date, "%*d/%d", &n1);
    sscanf(b.date, "%*d/%d", &n2);
    if(n1>n2) return 1;
    else if(n2>n1) return -1;
    sscanf(a.date, "%*d/%*d/%d", &n1);
    sscanf(b.date, "%*d/%*d/%d", &n2);
    if(n1>n2) return 1;
    else if(n2>n1) return -1;
    // ora
    sscanf(a.hourI, "%d", &n1);
    sscanf(b.date, "%d", &n2);
    if(n1>n2) return 1;
    else if(n2>n1) return -1;
    sscanf(a.hourI, "%*d:%d", &n1);
    sscanf(b.date, "%*d:%d", &n2);
    if(n1>n2) return 1;
    else if(n2>n1) return -1;
    sscanf(a.hourI, "%*d:%*d:%d", &n1);
    sscanf(b.date, "%*d:%*d:%d", &n2);
    if(n1>n2) return 1;
    else if(n2>n1) return -1;
    //pari
    return 0;
}

int is_a_less_then_b(dati a, dati b, order key){
    switch(key) {
        case data:
            return date_cmp(a, b);
        case tratta:
            return strcmp(a.id, b.id);
        case partenza:
            return strcmp(a.start, b.start);
        case arrivo:
            return strcmp(a.end, b.end);
    }
    return 1; // non dovrebbe essere possibile arrivare qua
}

void merge(dati* arr[], int l, int q, int r, order key){
    int i = l, j = q+1, k, num = r-l;
    dati* tmp[r-l + 1];
    
    for (k = 0; k <= num; k++)
        if (i > q)
            tmp[k] = arr[j++];
        else if (j > r)
            tmp[k] = arr[i++];
        else if (is_a_less_then_b(*arr[i], *arr[j], key) < 0)
            tmp[k] = arr[i++];
        else
            tmp[k] = arr[j++];
    
    for ( k = 0; k <= num; k++ )
        arr[k+l] = tmp[k];
    return;
}

void mergeSort(dati* arr[], int l, int r, order key){
    if(l>=r) return;
    int m = l + (r - l) / 2; // (l+r)/2, but avoids overflow
    
    mergeSort(arr, l, m, key);
    mergeSort(arr, m + 1, r, key);
    
    merge(arr, l, m, r, key);
}

bool isStart(char *source, char *target){
    while(*target != '\0'){
        if(*source == '\0' || *source != *target){
            return False;
        }
        target++;
        source++;
    }
    return True;
}

void isSub(dati* log[], int lenLog, char *target){
    for(int i = 0; i < lenLog; i++)
        if(isStart(log[i]->start, target))
            print_log_row(log[i], stdout);
}

void isSubDico(dati* log[], int lenLog, char *target){
    int i = 0, f = lenLog, medio;

    while(i <= f){
        medio = (i+f)/2;

        if(isStart(log[medio]->start, target)){
            i = medio;
            while(i!=0 && isStart(log[i -1]->start, target)) i--;
            f = medio;
            while(f!=lenLog && isStart(log[f +1]->start, target)) f++;
            
            for(i; i<=f; i++) print_log_row(log[i], stdout);
        
        }else{
            if(strcmp(log[medio]->start, target) > 0)
                f = medio -1;
            else i = medio +1;
        }
    }
}

comandi get_cmd(char par1[], bool config){
    char str[maxWord];
    scanf("%s", str);   
    if(strcmp("stampa", str)==0 && config){scanf("%s", par1); return stampa;}
    if(strcmp("data", str)==0)return o_data;
    if(strcmp("tratta", str)==0) return o_tratta;
    if(strcmp("partenza", str)==0) return o_partenza;
    if(strcmp("arrivo", str)==0) return o_arrivo;
    if(strcmp("stazione", str)==0 && config){scanf("%s", par1); return r_stazione;}
    if(strcmp("lettura", str)==0 && config){scanf("%s", par1); return r_lettura;}
    if(strcmp("stop", str) == 0 || strcmp("fine", str) == 0 || strcmp("cls", str) == 0 && config) return r_fine;
    return -1;
}

void options(dati** log, dati**** ordinamento, int* lenLog){
    char par[maxWord];
    int i;
    order isOrder = none;

    while(True){
        printf(">> ");
        fflush(stdin);
    
        switch(get_cmd(par, True)){
            case stampa:;
                comandi comando;
                if(strcmp(par, "file") == 0){
                    printf("Seleziona il tipo di ordinamento da stamapre: [lettura, data, tratta, partenza, arrivo, stazione].\n\t>> ");
                    comando = get_cmd(par, False);
                    FILE *fout;
                    if ((fout = fopen(fileOut, "w")) == NULL){
                        printf("Errore nell'apertura del file: \"fileOut\"");
                        return;
                    }
                    if((*ordinamento)[comando][0] == NULL){
                        printf("Ordinamento non ancora esistente\n");
                        break;
                    }
                    print_log((*ordinamento)[comando], *lenLog, fout);
                    fclose(fout);
                }
                else if(strcmp(par, "console") == 0){
                    printf("Seleziona il tipo di ordinamento da stamapre: [lettura, data, tratta, partenza, arrivo].\n\t>> ");
                    comando = get_cmd(par, False);
                    if(comando==-1) comando = 0;
                    if((*ordinamento)[comando][0] == NULL){
                        printf("Ordinamento non ancora esistente\n");
                        break;
                    }
                    print_log((*ordinamento)[comando], *lenLog, stdout);
                }
                else printf("Output di stampa non riconosciuto. Output disponibli: [file, console]\n");
                break;

            case o_data:
                if((*ordinamento)[data][0] == NULL){
                    copyArray((*ordinamento)[0], *lenLog, (*ordinamento)[data]);
                    mergeSort((*ordinamento)[data], 0, *lenLog-1, data);
                }
                else printf("Vettore gia' ordinameto secondo la data\n");
                break;

            case o_tratta:
                if((*ordinamento)[tratta][0] == NULL){
                    copyArray((*ordinamento)[0], *lenLog, (*ordinamento)[tratta]);
                    mergeSort((*ordinamento)[tratta], 0, *lenLog-1, tratta);
                }
                else printf("Vettore gia' ordinameto secondo la tratta\n");
                break;

            case o_partenza:
                if((*ordinamento)[partenza][0] == NULL){
                    copyArray((*ordinamento)[0], *lenLog, (*ordinamento)[partenza]);
                    mergeSort((*ordinamento)[partenza], 0, *lenLog-1, partenza);
                }
                else printf("Vettore gia' ordinameto secondo la stazione di partenza\n");

                break;

            case o_arrivo:
                if((*ordinamento)[arrivo][0] == NULL){
                    copyArray((*ordinamento)[0], *lenLog, (*ordinamento)[arrivo]);
                    mergeSort((*ordinamento)[arrivo], 0, *lenLog-1, arrivo);
                }
                else printf("Vettore gia' ordinameto secondo la stazione di arrivo\n");

                break;

            case r_stazione:
                ((*ordinamento)[partenza][0] == NULL) ? isSubDico((*ordinamento)[partenza], *lenLog, par) : isSub((*ordinamento)[partenza], *lenLog, par);
                break;
            
            case r_lettura:
                if(!resetAndStart(par, log, lenLog, ordinamento)){
                    printf("Rilettura file non andata a buon fine. Terminazione programma\n");
                    return;
                }
                break;
            
            case r_fine:
                printf("Terminazione programma");
                resetMem(log, lenLog, ordinamento);

                return;

            default:
                printf("Comando non riconosciuto\n");
        }
    }
}

int main(int argc, char* argv){
    dati* log;
    int lenLog;
    dati*** ordinamento/* [numOrdinamento][maxRow] */;
    if(!startStrutture(fileIn, &log, &lenLog, &ordinamento)) return -1;

    printf("Inizio programma:\n");
    printf("Comandi riconosciuti: stampa ['console', 'file'], data, tratta, partenza, arrivo, stazione, lettura 'file', stop\n");
    options(&log, &ordinamento, &lenLog);
    
    return 0;
}
