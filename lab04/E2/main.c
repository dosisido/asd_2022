/*
 * Riccardo Rosin 284211
 * Lab 4 Es 2
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define STRLEN (50 + 1)
#define FILE_OUT "out.txt"

typedef enum{false, true} bool;
typedef enum{inserimentoConsole, inserimentoFile, ricerca, cancellazione, stampa, fine} comandi;

typedef struct{
    char code[STRLEN];
    char name[STRLEN];
    char surname[STRLEN];
    char date[11];
    char street[STRLEN];
    char city[STRLEN];
    int cap;
} Item;

typedef struct node{
    Item item;
    struct node* next; 
} node;

const Item EMPTY_ITEM = {.code = '\0', .name = '\0', .surname = '\0', .date = '\0', .street = '\0', .city = '\0', .cap = 0};

int date_cmp(char* a, char* b){ // 1 se a > b
    /*   
     *   1 se 1 > 2
     *   0 se 1 = 2
     *  -1 se 1 < 2
     */
    int n1, n2;
    // data
    sscanf(a, "%*d/%*d/%d", &n1);
    sscanf(b, "%*d/%*d/%d", &n2);
    if(n1>n2) return 1;
    else if(n2>n1) return -1;
    sscanf(a, "%*d/%d", &n1);
    sscanf(b, "%*d/%d", &n2);
    if(n1>n2) return 1;
    else if(n2>n1) return -1;
    sscanf(a, "%d", &n1);
    sscanf(b, "%d", &n2);
    if(n1>n2) return 1;
    else if(n2>n1) return -1;
    return 0;
}

void inserisci(FILE* fin, node** start){
    node* thisNode = (node*) malloc(sizeof(node)), *tmp, *prev;
    Item* thisItem = &thisNode->item;

    fscanf(fin, "%s %s %s %s %s %s %d ",
        thisItem->code,
        thisItem->name,
        thisItem->surname,
        thisItem->date,
        thisItem->street,
        thisItem->city,
        &thisItem->cap
    );

    if(*start == NULL){ // se lista vuota
        thisNode->next = NULL;
        *start = thisNode;
        return;
    }
    
    for(tmp = *start, prev=NULL; tmp != NULL; prev = tmp, tmp = tmp->next){
        if(date_cmp(thisItem->date, tmp->item.date) == -1){
            // da inserire tra prev e tmp
            if(prev == NULL){
                thisNode->next = tmp;
                *start = thisNode;
                return;
            }else{
                thisNode->next = prev->next;
                prev->next = thisNode;
                return;
            }
        }
    }
    thisNode->next = NULL;
    prev->next = thisNode;

}

void stampaNodo(FILE* fout, node* tmp){
    fprintf(fout, "Nodo: %s %s %s %s %s %s %d\n",
            tmp->item.code,
            tmp->item.name,
            tmp->item.surname,
            tmp->item.date,
            tmp->item.street,
            tmp->item.city,
            tmp->item.cap
        );
}

void stampaFile(FILE *fout, node* start){
    if(start == NULL) return;

    for(node* tmp = start; tmp != NULL; tmp = tmp->next){
        stampaNodo(fout, tmp);
    }
}

node* ricercaKey(node* start, char* str, int* flag){
    for(node* tmp = start, *prev = NULL; tmp!=NULL; prev = tmp, tmp=tmp->next){
        if(strcmp(tmp->item.code, str)){
            *flag = true;
            return prev;
        }
    }
    *flag = false;
    return NULL;
}

node* estraiDate(node** head, char* date1, char* date2){
    node* this, *start, *prev;
    bool flag = false;
    for(this = *head, prev = NULL; this != NULL; prev = this, this = this->next){
        if(date_cmp(this->item.date, date1) >= 0){ // se la data che cerco è più piccola del nodo, quindi e prima di questo
            flag = true;
            start = prev;
            break;
        }
    }
    if(!flag) return NULL; // elemento non trovato

    flag = false;
    for(; this!=NULL; prev = this, this = this->next){
        if(date_cmp(this->item.date, date2) >= 0){ 
            break;
        }
    }

    // sequenza compresa tra start->next e prev compresi
    if(start == NULL){ // inizio è la head
        this = *head;
        *head = prev->next;
    }else{
        this = start->next;
        start->next = prev->next;
    }
    prev->next = NULL;

    return this;

}

void resetLista(node* thisNode){
    if(thisNode ==  NULL) return;
    if(thisNode->next != NULL) resetLista(thisNode->next);
    free(thisNode);
}

comandi get_cmd(char par1[], char par2[]){
    char str[STRLEN];
    scanf("%s", str);
    if(strcmp("inserimento", str)==0){
        scanf("%s", par1);
        if(strcmp("console", par1)==0) return inserimentoConsole; 
        if(strcmp("file", par1)==0){scanf("%s", par1); return inserimentoFile;} 
        return -1;
    }
    if(strcmp("ricerca", str)==0){scanf("%s", par1); return ricerca;}
    if(strcmp("cancellazione", str)==0){scanf("%s%s", par1, par2); return cancellazione;}
    if(strcmp("stampa", str)==0){scanf("%s", par1); return stampa;}
    if(strcmp("stop", str) == 0 || strcmp("fine", str) == 0 || strcmp("cls", str) == 0) return fine;
    return -1;
}

void options(){
    node* start = NULL, *this, *prev;
    char par1[STRLEN], par2[STRLEN];
    FILE* file;
    int i;

    if ((file = fopen("anag1.txt", "r+")) == NULL){
        printf("Errore nell'apertura del file: \"anag1.txt\"");
        exit(-1);
    }
    while(!feof(file)) inserisci(file, &start);
    fclose(file);
    if ((file = fopen("anag2.txt", "r+")) == NULL){
        printf("Errore nell'apertura del file: \"anag1.txt\"");
        exit(-1);
    }
    while(!feof(file)) inserisci(file, &start);
    fclose(file);


    while(true){
        printf(">> ");
        fflush(stdin);
    
        switch(get_cmd(par1, par2)){
            case inserimentoConsole:
                inserisci(stdin, &start);
                break;

            case inserimentoFile:;
                if ((file = fopen(par1, "r+")) == NULL){
                    printf("Errore nell'apertura del file: \"%s\"", par1);
                    exit(-1);
                }
                
                while(!feof(file)) inserisci(file, &start);
                
                fclose(file);
                break;


            case ricerca:;
                prev = ricercaKey(start, par1, &i);
                if(!i){
                    printf("Elemento non trovato\n");
                    break;                    
                }

                this = (prev == NULL) ? start : prev->next; // primo elemento o tutti gli altri
                
                // estrarre temporanemante l'elemento dalla lista
                printf("Elemento trovato: ");
                stampaNodo(stdout, this);
                printf("Lo vuoi cancellare? si | no\n");
                
                scanf("%s", par1);
                if(strcmp("si", par1) == 0){
                    printf("Eliminazione del dato\n");
                    if(prev == NULL) start = this->next;
                    else prev->next = this->next;
                    
                    this->next = NULL;
                    resetLista(this);
                    printf("Eliminazione completata\n");
                }else printf("Annullo operazione\n");

                break;
            
            case cancellazione:
                printf("Eliminazione dei seguenti nodi:\n");
                for(prev = estraiDate(&start, par1, par2), this = prev; this != NULL; this = this->next){
                    stampaNodo(stdout, this);
                }
                resetLista(prev);
                break;
            
            case stampa:
                if(strcmp("console", par1) == 0) stampaFile(stdout, start);
                else{
                    if ((file = fopen(par1, "w+")) == NULL){
                        printf("Errore nell'apertura del file: \"%s\"", par1);
                        exit(-1);
                    }
                    stampaFile(file, start);
                    fclose(file);
                }
                break;
            
            case fine:
                printf("Terminazione programma");
                resetLista(start);

                return;

            default:
                printf("Comando non riconosciuto\n");
        }
    }
}


int main(int argc, char* argv){
    printf("Inizio programma:\n");
    printf("Comandi riconosciuti: inserimento console | file, ricerca [codice], cancellazione [data1] [data2], stampa console | file [file], stop\n");
    options();

    return 0;
}