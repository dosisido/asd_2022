#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datetime.h"
#include "titolo.h"
#include "list.h"
#include "BST.h"

#define L 100


typedef enum {acquisizione, ricerca, visualizza, ricerca1, ricerca2, bilanciamento, stop} Comando;
typedef enum {false, true} Bool;

void leggiFile(char* nomeFile, List* CollezioneTitoli){
    FILE* fp = fopen(nomeFile, "r");
    if(fp == NULL){
        printf("Errore nell'apertura del file %s\n", nomeFile);
        return;
    }

    char s[LEN];
    int n, m, i, j, a, b;
    Titolo t;
    BST bst;
    Quotazione q, qtmp;
    KeyT kt;
    KeyQ kq;
    Datetime dt;

    fscanf(fp, "%d", &n);
    
    for(i=0; i<n; i++){
        t = TITOLO_init();
        bst;
    
        fscanf(fp, "%s", s);
        fscanf(fp, "%d", &m);

        TITOLO_setCode(t, s);
        kt = KEYT_get(t);

        // verificare se esiste ed eventualmente aggiungere il titolo
        if(!LIST_sortFind(*CollezioneTitoli, kt)){
            LIST_sortIns(*CollezioneTitoli, t);
        }else{
            Titolo tmp = t;
            t = LIST_sortSearch(*CollezioneTitoli, kt);
            TITOLO_free(tmp);
        }

        bst = TITOLO_getQt(t);

        for(j=0; j<m; j++){
            q = QUOTAZIONE_setNull();
            dt = DT_newDateFile(fp);
            fscanf(fp, "%d:%d %d %d", &a, &a, &a, &b);
            q = QUOTAZIONE_setDate(q, dt);

            if(QUOTAZIONE_eq( (qtmp = BST_search(bst, KEYQ_get(q))) , QUOTAZIONE_setVoid())){
                // non esiste record per la data
                q = QUOTAZIONE_setND(q, a*b, b);
                BST_insertLeafR(bst, q);
            }else{
                // esiste record per la data
                q = qtmp;
                q = BST_search(bst, KEYQ_get(q));
                q = QUOTAZIONE_addND(q, a*b, b);
                BST_update(bst, q);
            }
        }        
    }

    fclose(fp);
}

Comando leggiComando(){
    char s[L];
    scanf("%s", s);
    if(strcmp(s, "lettura") == 0) return acquisizione;
    if(strcmp(s, "ricerca") == 0) return ricerca;
    if(strcmp(s, "exit") == 0 || strcmp(s, "fine") == 0 || strcmp(s, "cls") == 0 || strcmp(s, "stop") == 0) return stop;
    return -1;
}


int main(int argc, char* argv[]){
    List CollezioneTitoli = LIST_init();
    Bool fine = false;
    char str[L];
    KeyT kt;
    KeyQ kq1, kq2;
    Titolo t, ttmp;
    BST quotazioni;
    Datetime dt;
    Quotazione q1, q2;

    leggiFile("F1.txt", &CollezioneTitoli);
    leggiFile("F2.txt", &CollezioneTitoli);
    leggiFile("F3.txt", &CollezioneTitoli);

    printf("Inizio programma. Comandi disponibili: lettura, ricerca, exit\n");
    while(!fine){
        printf(">>> ");
        switch(leggiComando()){
            case acquisizione:
                printf("Inserisci il nome del file di input: ");
                scanf("%s", str);
                
                leggiFile(str, &CollezioneTitoli);

                break;

            case ricerca:
                printf("Inserisci il codice del titolo azionario: ");
                scanf("%s", str);

                t = TITOLO_init();
                TITOLO_setCode(t, str);
                kt = KEYT_get(t);

                ttmp = LIST_sortSearch(CollezioneTitoli, kt);
                TITOLO_free(t);
                t = ttmp;
                if(TITOLO_eq(t, TITOLO_setVoid())){
                    printf("Titolo non trovato\n");
                    break;
                }

                printf("Titolo troato, desideri effetuare una delle seguenti operazioni?\n");
                printf("1) Visualizza quotazione in data\n");
                printf("2) Ricerca quotazione minima e massima in intervallo\n");
                printf("3) Ricerca quotazione minima e massima\n");
                printf("4) Bilanciamento BST\n");
                printf(">>> ");

                int c;
                scanf("%d", &c);
                if(c<1 || c>4) break;
                c+=1;

                quotazioni = TITOLO_getQt(t);

                switch(c){
                    case visualizza:
                        printf("Inserisci la data nella qaule effettuare la ricerca [format: YYYY/MM/GG]: ");
                        kq1 = KEYQ_scan(stdin);

                        q1 = BST_search(quotazioni, kq1);
                        if(QUOTAZIONE_eq(q1, QUOTAZIONE_setVoid())){
                            printf("Nessuna quotazione trovata per la data inserita");
                            break;
                        }

                        QUOTAZIONE_show(q1);

                        break;
                    
                    case ricerca1:
                        printf("Inserisci la data iniziale [format: YYYY/MM/GG]: ");
                        kq1 = KEYQ_scan(stdin);
                        printf("Inserisci la data finale [format: YYYY/MM/GG]: ");
                        kq2 = KEYQ_scan(stdin);

                        BST_searchQuotazioni(quotazioni, kq1, kq2, &q1, &q2);

                        if(QUOTAZIONE_eq(q1, QUOTAZIONE_setVoid()) || QUOTAZIONE_eq(q2, QUOTAZIONE_setVoid())){
                            printf("Nessuna quotazione trovata per l'intervallo inserito\n");
                            break;
                        }

                        printf("Quotazione minima %f in data ", QUOTAZIONE_value(q1)); DT_showDate((Datetime) KEYQ_get(q1)); printf("\n");
                        printf("Quotazione massima %f in data ", QUOTAZIONE_value(q2)); DT_showDate((Datetime) KEYQ_get(q2)); printf("\n");
                        
                        break;
                    
                    case ricerca2:
                        BST_searchQuotazioni(quotazioni, KEYQ_get(BST_min(quotazioni)), KEYQ_get(BST_max(quotazioni)), &q1, &q2);

                        if(QUOTAZIONE_eq(q1, QUOTAZIONE_setVoid()) || QUOTAZIONE_eq(q2, QUOTAZIONE_setVoid())){
                            printf("Nessuna quotazione trovata per l'intervallo inserito\n");
                            break;
                        }

                        printf("Quotazione minima %f in data ", QUOTAZIONE_value(q1)); DT_showDate((Datetime) KEYQ_get(q1)); printf("\n");
                        printf("Quotazione massima %f in data ", QUOTAZIONE_value(q2)); DT_showDate((Datetime) KEYQ_get(q2)); printf("\n");

                        break;
                    
                    case bilanciamento:
                        printf("Rapporto tra percorso massimo e minimo: %f\n", BST_LongestOverShortest(quotazioni));
                        printf("Vuoi bilanciare la BST? [y/n]: ");
                        scanf("%s", str);
                        
                        if(str[0] == 'n') break;

                        BST_balance(quotazioni);

                        break;
                }

                break;

            case stop:
                fine = true;
                break;
        }
    }

    // deallocazione memoria
    LIST_free(CollezioneTitoli);

    return 0;
}