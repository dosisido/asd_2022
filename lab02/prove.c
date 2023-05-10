#include<stdio.h>

int main(int argc, char* argv){
    int numero;

    numero = 4;

    printf("La numeroiabile vale %d\n", numero);

    numero = 5;

    printf("La numeroiabile vale %d\n", numero);


    int* p;

    p = &numero;

    printf("Il suo indirizzo vale %p\n", p);

    *p = 10;

    printf("La numeroiabile vale %d\n", numero);


    

    return 0;
}