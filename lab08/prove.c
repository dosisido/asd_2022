#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ROW 200


void init(char* filename){
    int V=0;
    char* row, *tmp;
    row = malloc(ROW*sizeof(char));
    tmp = malloc(ROW*sizeof(char));

    FILE *fp;
    if ((fp = fopen(filename, "r")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", filename);
        exit(-1);
    }
    
    while(!feof(fp)){
        fgets(row, ROW, fp);
        if(sscanf(row, "%s", tmp) == 1)
            V++;
    }

    printf("%d", V);

    fclose(fp);

    if ((fp = fopen(filename, "r")) == NULL){
        printf("Errore nell'apertura del file: \"%s\"", filename);
        exit(-1);
    }
    



    fclose(fp);
    free(row);
    free(tmp);


}

int main(int argc, char* argv[]){
    

    init("grafo.txt");


    return 0;
}